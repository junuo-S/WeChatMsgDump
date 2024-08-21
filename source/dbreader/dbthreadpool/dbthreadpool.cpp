﻿#include "dbthreadpool.h"

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QMutexLocker>

#include "databaseworker.h"

class JunuoWorkerPool
{
public:
	JunuoWorkerPool(const QString& dbName, int maxCount, JunuoDbThreadPool* threadPool)
		: m_maxCount(maxCount)
	{
		static const QString connectNameBase = "%1-thread-connection";
		for (size_t i = 0; i < m_maxCount; i++)
		{
			QThread* thread = new QThread(threadPool);
			DatabaseWorker* worker = new DatabaseWorker(dbName, connectNameBase.arg(i + 1));
			worker->moveToThread(thread);
			QObject::connect(thread, &QThread::start, worker, &DatabaseWorker::initializeDatabaseConnection, Qt::QueuedConnection);
			QObject::connect(worker, &DatabaseWorker::sigQueryResult, threadPool, &JunuoDbThreadPool::onQueryResult, Qt::QueuedConnection);
			QObject::connect(worker, &DatabaseWorker::sigQueryExecError, threadPool, &JunuoDbThreadPool::onQueryExecError, Qt::QueuedConnection);
			QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
			QObject::connect(thread, &QThread::finished, worker, &DatabaseWorker::deleteLater);
			thread->start();
			m_workerList.append(worker);
			m_workerThreadList.append(thread);
		}
	}

	~JunuoWorkerPool()
	{
		for (auto* thread : m_workerThreadList)
		{
			thread->quit();
		}
		for (auto* thread : m_workerThreadList)
		{
			thread->wait();
		}
	}

	DatabaseWorker* acquire()
	{
		m_mutex.lock();
		if (m_workerList.isEmpty())
			m_condition.wait(&m_mutex);
		auto worker = m_workerList.front();
		m_mutex.unlock();
		return worker;
	}

	void release(DatabaseWorker* worker)
	{
		QMutexLocker locker(&m_mutex);
		m_workerList.append(worker);
		m_condition.notify_all();
	}

private:
	QList<DatabaseWorker*> m_workerList;
	QList<QThread*> m_workerThreadList;
	int m_maxCount = -1;
	QMutex m_mutex;
	QWaitCondition m_condition;
};

struct JunuoDbThreadPool::Data
{
	std::unique_ptr<JunuoWorkerPool> workerPool = nullptr;
};

JunuoDbThreadPool::JunuoDbThreadPool(const QString& dbName, int maxCount /*= -1*/, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, data(new Data)
{
	data->workerPool.reset(new JunuoWorkerPool(dbName, maxCount > 0 ? maxCount : QThread::idealThreadCount(), this));
}

JunuoDbThreadPool::~JunuoDbThreadPool()
{
	
}

void JunuoDbThreadPool::executeQuery(const QString& sql)
{
	QThread* thread = QThread::create([this, sql]()
		{
			auto worker = data->workerPool->acquire();
			QMetaObject::invokeMethod(worker, "executeQuery", Q_ARG(const QString&, sql));
		});
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
	thread->start();
}

void JunuoDbThreadPool::onQueryResult(QVariantList result)
{
	if (auto worker = qobject_cast<DatabaseWorker*>(sender()))
		data->workerPool->release(worker);
}

void JunuoDbThreadPool::onQueryExecError(QString sql, QString errorMsg)
{

}

