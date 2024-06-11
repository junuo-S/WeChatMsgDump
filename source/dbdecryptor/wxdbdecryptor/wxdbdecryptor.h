#pragma once

#include <QStringList>
#include <QObject>
#include <vector>

enum WeChatDbType;

class WxDBDecryptor : public QObject
{
	Q_OBJECT

	using WeChatDbTypeList = std::vector<WeChatDbType>;
public:
	WxDBDecryptor(WeChatDbTypeList typeList, const QString& inputPath = QString(), const QString& outputPath = QString());
	bool beforeDecrypt();
	void beginDecrypt() const;
	size_t getTotalDBFileCount() const;

signals:
	void sigDecryptDoneOneFile(bool isSuccess) const;
	void sigDecryptAllDone() const;

private:
	QStringList getDbPathByWeChatDbType() const;
	QStringList getDbFiles(const QString& pattern, bool isRegular = true, const QString& subDir = QString()) const;
	bool decrypt(const QString& inputFilePath, const QString& outputFilePath) const;
	QByteArray pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const;
	QByteArray aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const;
	WeChatDbTypeList m_typeList;
	QString m_outputPath;
	QString m_inputPath;
	QStringList m_dbFileList;
};
