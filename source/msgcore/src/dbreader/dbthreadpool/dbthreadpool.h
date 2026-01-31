#pragma once

#include <QObject>

#include <QList>
#include <QMutex>
#include <QString>
#include <QVariant>
#include <QWaitCondition>

class DatabaseWorker;
class QThread;

class JunuoDbThreadPool : public QObject
{
	Q_OBJECT
public:
	JunuoDbThreadPool(const QString& dbName, int maxCount = -1, QObject* parent = nullptr);
	~JunuoDbThreadPool();
	void executeQuery(const QString& sql, QObject* receiver, const char* method, const QVariant& context = QVariant());

private:
	DatabaseWorker* acquireWorker();
	void releaseWorker(DatabaseWorker* worker);

	void onQueryFinished();

	QString m_dbName;
	QList<DatabaseWorker*> m_workerList;
	QList<QThread*> m_workerThreadList;
	int m_maxCount = -1;
	QMutex m_mutex;
	QWaitCondition m_condition;
};
