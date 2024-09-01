#include "databaseworker.h"

#include <QThread>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>

DatabaseWorker::DatabaseWorker(const QString& dbName, const QString& connectionName, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, m_dbName(dbName)
	, m_connectionName(connectionName)
{

}

DatabaseWorker::~DatabaseWorker()
{
	QSqlDatabase db = QSqlDatabase::database(m_connectionName);
	if (db.isOpen())
	{
		db.close();
		QSqlDatabase::removeDatabase(m_connectionName);
	}
}

void DatabaseWorker::initializeDatabaseConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
	db.setDatabaseName(m_dbName);
	if (!db.open())
	{
		emit sigDbOpenError(QString::number(reinterpret_cast<uintptr_t>(QThread::currentThreadId()), 16), db.lastError().text());
	}
}

void DatabaseWorker::executeQuery(const QString& sql, QObject* receiver, const char* method, const QVariant& context /*= QVariant()*/)
{
	QSqlDatabase db = QSqlDatabase::database(m_connectionName);
	QSqlQuery query(db);
	QVariantList result;
	do 
	{
		if (!query.exec(sql))
			break;
		while (query.next())
		{
			QVariantMap row;
			for (size_t i = 0; i < query.record().count(); i++)
			{
				row.insert(query.record().fieldName(i), query.value(i));
			}
			result.append(row);
		}
	} while (false);
	QMetaObject::invokeMethod(receiver, method, Qt::QueuedConnection, Q_ARG(QVariantList, result), Q_ARG(const QVariant&, context));
	emit sigQueryFinished();
}
