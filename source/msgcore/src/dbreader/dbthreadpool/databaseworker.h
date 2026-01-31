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
	void executeQuery(const QString& sql, QObject* receiver, const char* method, const QVariant& context = QVariant());

signals:
	void sigDbOpenError(QString threadId, QString errorMsg);
	void sigQueryFinished();

private:
	QString m_dbName;
	QString m_connectionName;
};
