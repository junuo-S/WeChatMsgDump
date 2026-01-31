#include "dbthreadpool.h"

#include <QThreadPool>
#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QMutexLocker>
#include <QMetaObject>

#include "databaseworker.h"

JunuoDbThreadPool::JunuoDbThreadPool(const QString& dbName, int maxCount /*= -1*/, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, m_dbName(dbName)
{
	m_maxCount = maxCount > 0 ? maxCount : QThread::idealThreadCount();

	static const QString connectNameBase = "%1-thread-connection";
	for (int i = 0; i < m_maxCount; i++)
	{
		QThread* thread = new QThread(this);
		DatabaseWorker* worker = new DatabaseWorker(m_dbName, connectNameBase.arg(i + 1));
		worker->moveToThread(thread);
		QObject::connect(thread, &QThread::started, worker, &DatabaseWorker::initializeDatabaseConnection, Qt::QueuedConnection);
		QObject::connect(worker, &DatabaseWorker::sigQueryFinished, this, &JunuoDbThreadPool::onQueryFinished, Qt::QueuedConnection);
		thread->start();
		m_workerList.append(worker);
		m_workerThreadList.append(thread);
	}
}

JunuoDbThreadPool::~JunuoDbThreadPool()
{
	for (auto* thread : m_workerThreadList)
	{
		thread->quit();
	}
	for (auto* thread : m_workerThreadList)
	{
		thread->wait();
	}

	for (auto* worker : m_workerList)
	{
		worker->moveToThread(QThread::currentThread());
		delete worker;
	}
	m_workerList.clear();

	for (auto* thread : m_workerThreadList)
	{
		delete thread;
	}
	m_workerThreadList.clear();
}

DatabaseWorker* JunuoDbThreadPool::acquireWorker()
{
	QMutexLocker locker(&m_mutex);
	while (m_workerList.isEmpty())
	{
		m_condition.wait(&m_mutex);
	}
	return m_workerList.takeFirst();
}

void JunuoDbThreadPool::releaseWorker(DatabaseWorker* worker)
{
	QMutexLocker locker(&m_mutex);
	m_workerList.append(worker);
	m_condition.notify_all();
}

void JunuoDbThreadPool::executeQuery(const QString& sql, QObject* receiver, const char* method, const QVariant& context /*= QVariant()*/)
{
	auto queryAsyncCall = [this, sql, receiver, method, context]()
		{
			auto worker = acquireWorker();
			if (worker)
				QMetaObject::invokeMethod(worker, "executeQuery", Qt::QueuedConnection, Q_ARG(const QString&, sql), Q_ARG(QObject*, receiver), Q_ARG(const char*, method), Q_ARG(const QVariant&, context));
		};
	QThreadPool::globalInstance()->start(queryAsyncCall);
}

void JunuoDbThreadPool::onQueryFinished()
{
	if (auto worker = qobject_cast<DatabaseWorker*>(sender()))
		releaseWorker(worker);
}
