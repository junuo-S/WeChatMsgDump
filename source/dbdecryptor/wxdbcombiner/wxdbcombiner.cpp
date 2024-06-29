#include "wxdbcombiner.h"

#include <QStandardPaths>
#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QRegularExpression>

#include "wxmemoryreader/wxmemoryreader.h"

static constexpr const char* gs_mergeDBConnectionName = "merge_db";
static constexpr const char* gs_decryptedDBConnectionName = "decrypted_db";
static constexpr const char* gs_sqliteType = "QSQLITE";

WxDBCombiner::WxDBCombiner(const QStringList& decryptedFilesPath, const QString& mergeOutputFilePath, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, m_decryptedFilesPath(decryptedFilesPath)
	, m_mergeOutputFilePath(mergeOutputFilePath)
{
	if (m_mergeOutputFilePath.isEmpty() && !m_decryptedFilesPath.isEmpty())
	{
		auto firstFilePath = m_decryptedFilesPath.at(0);
		firstFilePath = firstFilePath.replace("\\\\", "/").replace("\\", "/");
		size_t wxidIndex = firstFilePath.indexOf("/wxid_");
		size_t nextSeparatorIndex = firstFilePath.indexOf("/", wxidIndex + 1);
		m_mergeOutputFilePath = firstFilePath.left(nextSeparatorIndex) % "/merge_db.db";
	}
}

void WxDBCombiner::beginCombine()
{
	emit sigCombineStarted(m_decryptedFilesPath.size());
	std::remove(m_mergeOutputFilePath.toStdString().c_str());
	auto mergeDb = QSqlDatabase::addDatabase(gs_sqliteType, gs_mergeDBConnectionName);
	mergeDb.setDatabaseName(m_mergeOutputFilePath);
	if (!mergeDb.open())
	{
		emit sigCombineFinished(false);
		return;
	}
	for (const auto& dbFilePath : m_decryptedFilesPath)
	{
		mergeDb.transaction();
		combineDBFile(dbFilePath);
		mergeDb.commit();
	}
	emit sigCombineFinished(true);
	mergeDb.close();
	QSqlDatabase::removeDatabase(gs_mergeDBConnectionName);
}

void WxDBCombiner::combineDBFile(const QString& path)
{
	QSqlQuery mergeQuery(QSqlDatabase::database(gs_mergeDBConnectionName));
	auto decryptedDB = QSqlDatabase::addDatabase(gs_sqliteType, gs_decryptedDBConnectionName);
	decryptedDB.setDatabaseName(path);
	bool isSuccess = false;
	do 
	{
		if (!decryptedDB.open())
			break;
		QString tableSql = "select tbl_name,sql from sqlite_master where  type='table' and tbl_name!='sqlite_sequence'";
		QSqlQuery tableQuery(decryptedDB);
		if (!tableQuery.exec(tableSql))
			break;
		while (tableQuery.next())
		{
			auto sql = tableQuery.value("sql").toString();
			sql = sql.replace("CREATE TABLE ", "CREATE TABLE IF NOT EXISTS ", Qt::CaseInsensitive);
			const QString primaryKey = "PRIMARY KEY";
			int start = sql.indexOf(primaryKey);
			int end = sql.indexOf(",", start);
			if (start >= 0 && end >= 0 && end > start)
				sql.remove(start, end - start);
			if (QSqlDatabase::database(gs_mergeDBConnectionName).exec(sql).lastError().isValid())
				continue;
			QSqlQuery contentQuery(decryptedDB);
			QString tableName = tableQuery.value("tbl_name").toString();
			contentQuery.exec(QString("SELECT * FROM %1").arg(tableName));
			if (contentQuery.next())
			{
				QStringList columns;
				QStringList placeholders;
				for (int i = 0; i < contentQuery.record().count(); ++i)
				{
					columns << contentQuery.record().fieldName(i);
					placeholders << "?";
				}

				// 创建包含 NULL 值比较的 UNIQUE 索引
				QString indexName = tableName % "_unique_index";
				QStringList coalesceColumns;
				for (const auto& c : columns)
					coalesceColumns.append(QString("COALESCE(%1, '')").arg(c));
				QString coalesceColumn = coalesceColumns.join(",");  // 将 NULL 值转换为 ''
				QString indexSql = QString("CREATE UNIQUE INDEX IF NOT EXISTS ") % indexName % " ON " % tableName % " (" % coalesceColumn % ")";
				if (!mergeQuery.exec(indexSql))
					continue;

				QVector<QVariantList> values;
				values.resize(columns.size());
				do
				{
					auto record = contentQuery.record();
					for (int i = 0; i < record.count(); ++i)
					{
						values[i].append(record.value(i));
					};
				} while (contentQuery.next());
				QString insertSql = QString("INSERT %4 INTO %1 (%2) VALUES (%3)")
					.arg(tableName)
					.arg(columns.join(", "))
					.arg(placeholders.join(", "))
					.arg(tableName == "Name2ID" ? "OR IGNORE" : "");
				mergeQuery.prepare(insertSql);
				for (size_t i = 0; i < columns.size(); i++)
					mergeQuery.bindValue(i, values.at(i));
				if (!mergeQuery.execBatch())
				{
					auto error = mergeQuery.lastError().text();
					qDebug() << error;
					continue;
				}
			}
			isSuccess = true;
		}
	} while (false);
	decryptedDB.close();
	QSqlDatabase::removeDatabase(gs_decryptedDBConnectionName);
	emit sigCombineOneFinished(isSuccess);
}

