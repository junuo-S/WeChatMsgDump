﻿#pragma once

#include "defines.h"

#include <QStringList>
#include <QObject>
#include <vector>
#include <QMutex>

class WxDBDecryptor : public QObject
{
	Q_OBJECT

	using WeChatDbTypeList = std::vector<WeChatDbType>;
public:
	WxDBDecryptor(WeChatDbTypeList typeList, const QString& inputPath, const QString& outputPath);
	bool beforeDecrypt();
	void beginDecrypt();
	size_t getTotalDBFileCount() const;
	QStringList getOutputDBFiles() const;

signals:
	void sigUpdateProgress(int current, int total);
	void sigDecryptFinished() const;

private:
	QStringList getDbPathByWeChatDbType() const;
	QStringList getDbFiles(const QString& pattern, bool isRegular = true, const QString& subDir = QString()) const;
	bool decryptFile(const QString& inputFilePath, const QString& outputFilePath);
	QByteArray pbkdf2_hmac(const QByteArray& password, const QByteArray& salt, int iterations, int dklen) const;
	QByteArray aesCbcDecrypt(const QByteArray& key, const QByteArray& iv, const QByteArray& cipherText) const;
	WeChatDbTypeList m_typeList;
	QString m_outputPath;
	QString m_inputPath;
	QStringList m_inputDBFileList;
	QStringList m_outputDBFileList;
	QMutex m_mutex;
	std::atomic<int> m_currentProgress;
};
