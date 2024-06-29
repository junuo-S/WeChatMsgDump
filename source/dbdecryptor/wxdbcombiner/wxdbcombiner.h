#pragma once

#include <QObject>

class WxDBCombiner : public QObject
{
	Q_OBJECT
public:
	WxDBCombiner(const QStringList& decryptedFilesPath, const QString& mergeOutputFilePath = QString(), QObject* parent = nullptr);
	void beginCombine();

signals:
	void sigCombineFinished(bool isSuccess);
	void sigCombineOneFinished(bool isSuccess);
	void sigCombineStarted(int totalCount);

private:
	void combineDBFile(const QString& path);

	QStringList m_decryptedFilesPath;
	QString m_mergeOutputFilePath;
};
