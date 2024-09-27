#pragma once

#include <QObject>

class WxDBCombiner : public QObject
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
