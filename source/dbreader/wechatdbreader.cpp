#include "wechatdbreader.h"

#include "dbthreadpool/dbthreadpool.h"

struct WechatDbReader::Data
{
	QString dbName;
	JunuoDbThreadPool* dbThreadPool = nullptr;
};

WechatDbReader::WechatDbReader(const QString& dbName, QObject* parent /*= nullptr*/)
	: QObject(parent)
	, data(new Data)
{
	data->dbName = dbName;
	data->dbThreadPool = new JunuoDbThreadPool(dbName, -1, this);
}

WechatDbReader::~WechatDbReader()
{

}

void WechatDbReader::selectAllStrTalkerFromMSG(QObject* reciver, const char* method, const QVariantMap& param /* = QVariantMap()*/)
{
	const QString sql = "select distinct(strTalker) from MSG;";
	data->dbThreadPool->executeQuery(sql, reciver, method);
}

void WechatDbReader::selectHeadImageByUserName(QObject* reciver, const char* method, const QVariantMap& param /*= QVariantMap()*/)
{
	const QString sql = "select bigHeadImgUrl, smallHeadImgUrl from ContactHeadImgUrl where usrName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value("userName").toString()), reciver, method);
}

void WechatDbReader::selectRemarkByUserName(QObject* reciver, const char* method, const QVariantMap& param /*= QVariantMap()*/)
{
	const QString sql = "elect Remark from Contact where UserName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value("userName").toString()), reciver, method);
}

