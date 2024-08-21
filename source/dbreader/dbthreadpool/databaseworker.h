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
	void executeQuery(const QString& sql);

signals:
	void sigDbOpenError(QString threadId, QString errorMsg);
	void sigQueryResult(QVariantList result);
	void sigQueryExecError(QString sql, QString errorMsg);

private:
	QString m_dbName;
	QString m_connectionName;
};
