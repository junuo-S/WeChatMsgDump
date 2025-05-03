#pragma once

#include <QString>

class JAbstractWeChatDBDecryptor
{
public:
	virtual ~JAbstractWeChatDBDecryptor() {}
	virtual void SetParam(const QString& inputFilePath, const QString& outputFilePath, const QString& key);
	virtual bool Decrypt() = 0;

protected:
	QByteArray pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const;
	QByteArray aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const;

	QString m_inputFilePath;
	QString m_outputFilePath;
	QString m_key;
};
