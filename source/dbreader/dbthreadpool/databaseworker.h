#pragma once

#include <QObject>
#include <QSqlQuery>

class DatabaseWorker : public QObject
{
	Q_OBJECT
public:
	DatabaseWorker(const QString& dbName, const QString& connectionName, QObject* parent = nullptr);
	~DatabaseWorker();

public slots:
	void initializeDatabaseConnection();
	void executeQuery(const QString& sql, QObject* reciver, const char* method);

signals:
	void sigDbOpenError(QString threadId, QString errorMsg);
	void sigQueryFinished();

private:
	QString m_dbName;
	QString m_connectionName;
};
