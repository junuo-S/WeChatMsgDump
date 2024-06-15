#include "wxdbdecryptor.h"

#include "wxmemoryreader/wxmemoryreader.h"

#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QThreadPool>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>

static constexpr int gs_keySize = 32;
static constexpr int gs_pageSize = 4096;
static constexpr const char* gs_sqliteFileHeaderHex = "53514c69746520666f726d6174203300";

WxDBDecryptor::WxDBDecryptor(WeChatDbTypeList typeList, const QString& inputPath /* = QString()*/, const QString& outputPath /*= QString()*/)
	: m_typeList(typeList)
	, m_outputPath(outputPath)
	, m_inputPath(inputPath)
{
	if (outputPath.isEmpty() || !QDir(outputPath).exists())
		m_outputPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	if (inputPath.isEmpty() || !QDir(inputPath).exists())
		m_inputPath = QString::fromStdWString(WxMemoryReader::instance()->getWxDataPaths().at(0));
}

void WxDBDecryptor::beginDecrypt() const
{
	auto threadPool = QThreadPool::globalInstance();
	for (const auto& file : m_dbFileList)
	{
		threadPool->start([&file, this]() 
			{ 
				decrypt(file, m_outputPath + file.mid(file.lastIndexOf("/wxid_")));
			});
	}
	threadPool->waitForDone();
	emit sigDecryptAllDone();
}

bool WxDBDecryptor::beforeDecrypt()
{
	m_dbFileList = getDbPathByWeChatDbType();
	return !m_dbFileList.isEmpty();
}

size_t WxDBDecryptor::getTotalDBFileCount() const
{
	return m_dbFileList.size();
}

QStringList WxDBDecryptor::getDbPathByWeChatDbType() const
{
	QStringList ret;
	for (const auto type : m_typeList)
	{
		QString pattern, subDir;
		bool isRegular = true;
		switch (type)
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
		ret.append(getDbFiles(pattern, isRegular, subDir));
	}
	return ret;
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

bool WxDBDecryptor::decrypt(const QString& inputFilePath, const QString& outputFilePath) const
{
	if (!QFileInfo::exists(inputFilePath))
	{
		emit sigDecryptDoneOneFile(false);
		return false;
	}
	const std::string key = WxMemoryReader::instance()->getSecretKey();
	if (key.length() != 64)
	{
		emit sigDecryptDoneOneFile(false);
		return false;
	}
	QFile inputFile(inputFilePath);
	if (!inputFile.open(QIODevice::ReadOnly))
	{
		inputFile.close();
		emit sigDecryptDoneOneFile(false);
		return false;
	}
	size_t inputFileSize = inputFile.size();
	uchar* inputFileMapPtr = inputFile.map(0, inputFileSize);
	inputFile.close();
	QByteArray salt = QByteArray::fromRawData((const char*)inputFileMapPtr, 16);
	QByteArray first = QByteArray::fromRawData((const char*)(inputFileMapPtr + 16), gs_pageSize - 16);
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
	{
		emit sigDecryptDoneOneFile(false);
		return false;
	}
	QFile outputFile(outputFilePath);
	QFileInfo fileInfo(outputFilePath);
	if (!fileInfo.exists())
		QDir().mkpath(fileInfo.absoluteDir().absolutePath());
	if (!outputFile.open(QIODevice::WriteOnly))
	{
		outputFile.close();
		emit sigDecryptDoneOneFile(false);
		return false;
	}
	outputFile.write(QByteArray::fromHex(gs_sqliteFileHeaderHex));
	QByteArray tbList = QByteArray::fromRawData((const char*)(inputFileMapPtr + 16), gs_pageSize - 16);
	outputFile.write(aesCbcDecrypt(byteHMac, tbList.mid(tbList.size() - 48, 16), tbList.left(tbList.size() - 48)));
	outputFile.write(tbList.right(48));
	for (size_t i = gs_pageSize; i < inputFileSize; i += gs_pageSize)
	{
		tbList = QByteArray::fromRawData((const char*)(inputFileMapPtr + i), gs_pageSize);
		outputFile.write(aesCbcDecrypt(byteHMac, tbList.mid(tbList.size() - 48, 16), tbList.left(tbList.size() - 48)));
		outputFile.write(tbList.right(48));
	}
	outputFile.close();
	emit sigDecryptDoneOneFile(true);
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

