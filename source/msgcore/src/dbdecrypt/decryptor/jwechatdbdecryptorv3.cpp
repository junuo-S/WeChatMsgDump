#include "jwechatdbdecryptorv3.h"

#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>

#include "dbdecrypt/jwechatdbdecryptmanager.h"

static constexpr int gs_keySize = 32;
static constexpr int gs_pageSize = 4096;
static constexpr const char* const gs_sqliteFileHeaderHex = "53514c69746520666f726d6174203300";

bool JWeChatDBDecryptorV3::Decrypt()
{
	if (!QFileInfo::exists(m_inputFilePath) || m_key.length() != 64)
		return false;
	QFile inputFile(m_inputFilePath);
	if (!inputFile.open(QIODevice::ReadOnly))
		return false;
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
	QByteArray password = QByteArray::fromHex(m_key.toUtf8());
	auto byteHMac = pbkdf2_hmac(password, salt, 64000, gs_keySize);
	auto macKey = pbkdf2_hmac(byteHMac, macSalt, 2, gs_keySize);
	QMessageAuthenticationCode hashMac(QCryptographicHash::Sha1);
	hashMac.setKey(macKey);
	hashMac.addData(first.left(4064 - 16));
	hashMac.addData(QByteArray::fromHex("01000000"));
	if (hashMac.result().compare(first.mid(first.size() - 32, 20)) != 0)
		return false;
	QFile outputFile(m_outputFilePath);
	QFileInfo fileInfo(m_outputFilePath);
	if (!fileInfo.exists())
		QDir().mkpath(fileInfo.absoluteDir().absolutePath());
	if (!outputFile.open(QIODevice::WriteOnly))
		return false;
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
	return true;
}

DECLAREFACTORY_DECRYPTOR(WeChatMajorVersion::Version_3, JWeChatDBDecryptorV3)
