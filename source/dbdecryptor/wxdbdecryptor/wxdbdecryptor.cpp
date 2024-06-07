#include "wxdbdecryptor.h"

#include "defines.h"
#include "wxmemoryreader/wxmemoryreader.h"

#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>

static constexpr int gs_keySize = 32;
static constexpr int gs_pageSize = 4096;
static constexpr char* gs_sqliteFileHeaderHex = "53514c69746520666f726d6174203300";

WxDBDecryptor::WxDBDecryptor(WeChatDbType type, const QString& inputPath /* = QString()*/, const QString& outputPath /*= QString()*/)
	: m_type(type)
	, m_outputPath(outputPath)
	, m_inputPath(inputPath)
{
	if (outputPath.isEmpty() || !QDir(outputPath).exists())
		m_outputPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	if (inputPath.isEmpty() || !QDir(inputPath).exists())
		m_inputPath = QString::fromStdWString(WxMemoryReader::instance()->getWxDataPaths().at(0));
}

bool WxDBDecryptor::beginDecrypt() const
{
	QStringList dbFiles = getDbPathByWeChatDbType();
	for (const auto& file : dbFiles)
	{
		if (!decrypt(file, m_outputPath + file.mid(file.lastIndexOf("/wxid_"))))
			return false;
	}
	return true;
}

QStringList WxDBDecryptor::getDbPathByWeChatDbType() const
{
	QString pattern, subDir;
	bool isRegular = true;
	switch (m_type)
	{
	case WeChatDbType::_MSG:
		subDir = "Multi";
		pattern = "^MSG\\d+\\.db$";
		break;
	case WeChatDbType::MediaMSG:
		subDir = "Multi";
		pattern = "^MediaMSG\\d+\\.db$";
		break;
	case WeChatDbType::MicroMsg:
		pattern = "MicroMsg.db";
		isRegular = false;
		break;
	case WeChatDbType::OpenIMContact:
		pattern = "OpenIMContact.db";
		isRegular = false;
		break;
	case WeChatDbType::OpenIMMedia:
		pattern = "OpenIMMedia.db";
		isRegular = false;
		break;
	case WeChatDbType::OpenIMMsg:
		pattern = "OpenIMMsg.db";
		isRegular = false;
		break;
	default:
		break;
	}
	return getDbFiles(pattern, isRegular, subDir);
}

QStringList WxDBDecryptor::getDbFiles(const QString& pattern, bool isRegular /*= true*/, const QString& subDir /* = QString()*/) const
{
	QStringList ret;
	QDir dir(m_inputPath + "/Msg");
	if (!dir.exists())
		return ret;
	if (!subDir.isEmpty())
		dir.cd(subDir);
	auto allFiles = dir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::SortFlag::Name);
	for (const auto& file : allFiles)
	{
		if (isRegular ? QRegularExpression(pattern).match(file).hasMatch() : file == pattern)
			ret.append(dir.absoluteFilePath(file));
	}
	return ret;
}

bool WxDBDecryptor::decrypt(const QString& inputFile, const QString& outputFile) const
{
	if (!QFileInfo::exists(inputFile))
		return false;
	const std::string key = WxMemoryReader::instance()->getSecretKey();
	if (key.length() != 64)
		return false;
	QFile dbFile(inputFile);
	if (!dbFile.open(QIODevice::ReadOnly))
	{
		dbFile.close();
		return false;
	}
	size_t dbFileSize = dbFile.size();
	uchar* ptr = dbFile.map(0, dbFileSize);
	dbFile.close();
	QByteArray salt = QByteArray::fromRawData((const char*)ptr, 16);
	QByteArray first = QByteArray::fromRawData((const char*)(ptr + 16), gs_pageSize - 16);
	QByteArray macSalt;
	for (const auto& ch : salt)
	{
		macSalt.append(ch ^ 58);
	}
	QByteArray password = QByteArray::fromHex(QByteArray::fromStdString(key));
	auto byteHMac = pbkdf2_hmac(password, salt, 64000, gs_keySize);
	auto macKey = pbkdf2_hmac(byteHMac, macSalt, 2, gs_keySize);
	QMessageAuthenticationCode hashMac(QCryptographicHash::Sha1);
	hashMac.setKey(macKey);
	hashMac.addData(first.left(4064 - 16));
	hashMac.addData(QByteArray::fromHex("01000000"));
	if (hashMac.result().compare(first.mid(first.size() - 32, 20)) != 0)
		return false;
	QFile out(outputFile);
	QFileInfo fileInfo(outputFile);
	if (!fileInfo.exists())
		QDir().mkpath(fileInfo.absoluteDir().absolutePath());
	if (!out.open(QIODevice::WriteOnly))
	{
		out.close();
		return false;
	}
	out.write(QByteArray::fromHex(gs_sqliteFileHeaderHex));
	QByteArray tbList = QByteArray::fromRawData((const char*)(ptr + 16), gs_pageSize - 16);
	out.write(aesCbcDecrypt(byteHMac, tbList.mid(tbList.size() - 48, 16), tbList.left(tbList.size() - 48)));
	out.write(tbList.right(48));
	for (size_t i = gs_pageSize; i < dbFileSize; i += gs_pageSize)
	{
		tbList = QByteArray::fromRawData((const char*)(ptr + i), gs_pageSize);
		out.write(aesCbcDecrypt(byteHMac, tbList.mid(tbList.size() - 48, 16), tbList.left(tbList.size() - 48)));
		out.write(tbList.right(48));
	}
	out.close();
	return true;
}

QByteArray WxDBDecryptor::pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const
{
	QByteArray key(dklen, 0);
	int result = PKCS5_PBKDF2_HMAC(
		password.constData(),
		password.size(),
		reinterpret_cast<const unsigned char*>(salt.constData()),
		salt.size(),
		iterations,
		EVP_sha1(),
		dklen,
		reinterpret_cast<unsigned char*>(key.data())
	);
	if (result != 1)
		return QByteArray();
	return key;
}

QByteArray WxDBDecryptor::aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const
{
	if (cipherText.isEmpty())
		return QByteArray();

	AES_KEY aesKey;
	AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(key.constData()),
		8 * key.length(),
		&aesKey);
	QByteArray plainBytes;
	plainBytes.resize(cipherText.length());
	AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(cipherText.constData()),
		reinterpret_cast<unsigned char*>(plainBytes.data()),
		cipherText.size(),
		&aesKey,
		reinterpret_cast<unsigned char*>(const_cast<char*>(iv.data())),
		AES_DECRYPT);
	return plainBytes;
}

