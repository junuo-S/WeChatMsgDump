#pragma once

#ifndef DBDECRYPTOR_EXPORT

#ifdef _DB_DECRYPTOR_MODULE_
#define DBDECRYPTOR_EXPORT __declspec(dllexport)
#else
#define DBDECRYPTOR_EXPORT __declspec(dllimport)
#endif // _DB_DECRYPTOR_MODULE_

#endif // DBDECRYPTOR_EXPORT

#include <QObject>

class DBDECRYPTOR_EXPORT WxDBCombiner : public QObject
{
	Q_OBJECT
public:
	WxDBCombiner(const QStringList& decryptedFilesPath, const QString& mergeOutputFilePath = QString(), QObject* parent = nullptr);
	QString beginCombine();

signals:
	void sigCombineFinished(bool isSuccess);
	void sigUpdateProgress(int current, int total);
	void sigCombineStarted();

private:
	void combineDBFile(const QString& path);

	QStringList m_decryptedFilesPath;
	QString m_mergeOutputFilePath;
	int m_currentProgress = 0;
};
