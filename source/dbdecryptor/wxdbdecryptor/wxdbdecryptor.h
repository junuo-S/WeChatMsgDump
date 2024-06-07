#pragma once

#include <QString>

enum WeChatDbType;

class WxDBDecryptor
{
public:
	WxDBDecryptor(WeChatDbType type, const QString& inputPath = QString(), const QString& outputPath = QString());
	bool beginDecrypt() const;

private:
	QStringList getDbPathByWeChatDbType() const;
	QStringList getDbFiles(const QString& pattern, bool isRegular = true, const QString& subDir = QString()) const;
	bool decrypt(const QString& inputFile, const QString& outputFile) const;
	QByteArray pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const;
	QByteArray aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const;
	WeChatDbType m_type;
	QString m_outputPath;
	QString m_inputPath;
};
