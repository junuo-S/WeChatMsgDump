#include "jabstractwechatdbdecryptor.h"

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void JAbstractWeChatDBDecryptor::SetParam(const QString& inputFilePath, const QString& outputFilePath, const QString& key)
{
	m_inputFilePath = inputFilePath;
	m_outputFilePath = outputFilePath;
	m_key = key;
}

QByteArray JAbstractWeChatDBDecryptor::pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const
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

QByteArray JAbstractWeChatDBDecryptor::aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const
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
