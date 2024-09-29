#include "wxdbcombiner.h"

#include <QStandardPaths>
#include <QSqlDataBase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QRegularExpression>

#include "wxmemoryreader/wxmemoryreader.h"

static constexpr const char* const gs_mergeDBConnectionName = "merge_db";
static constexpr const char* const gs_decryptedDBConnectionName = "decrypted_db";
static constexpr const char* const gs_sqliteType = "QSQLITE";

WxDBCombiner::WxDBCombiner(const QStringList& decryptedFilesPath, const QString& mergeOutputFilePath, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, m_decryptedFilesPath(decryptedFilesPath)
	, m_mergeOutputFilePath(mergeOutputFilePath)
{
}

QString WxDBCombiner::beginCombine()
{
	m_currentProgress = 0;
	std::remove(m_mergeOutputFilePath.toStdString().c_str());
	auto mergeDb = QSqlDatabase::addDatabase(gs_sqliteType, gs_mergeDBConnectionName);
	mergeDb.setDatabaseName(m_mergeOutputFilePath);
	if (!mergeDb.open())
	{
		emit sigCombineFailed();
		return QString();
	}
	emit sigUpdateProgress(m_currentProgress, m_decryptedFilesPath.size());
	for (const auto& dbFilePath : m_decryptedFilesPath)
	{
		mergeDb.transaction();
		combineDBFile(dbFilePath);
		mergeDb.commit();
	}
	emit sigCombineFinished();
	mergeDb.close();
	QSqlDatabase::removeDatabase(gs_mergeDBConnectionName);
	return m_mergeOutputFilePath;
}

void WxDBCombiner::combineDBFile(const QString& path)
{
	m_currentProgress++;
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
			QStringList columns;
			QStringList placeholders;
			if (contentQuery.exec(QString("PRAGMA table_info(%1);").arg(tableName)))
			{
				// ===== 建索引 start =====
				while (contentQuery.next())
				{
					columns << contentQuery.value("name").toString();
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
				// ===== 建索引 end =====
			}

			contentQuery.exec(QString("SELECT * FROM %1").arg(tableName));
			if (contentQuery.next())
			{
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
					.arg(true || tableName == "Name2ID" ? "OR IGNORE" : "");
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
	emit sigUpdateProgress(m_currentProgress, m_decryptedFilesPath.size());
}

