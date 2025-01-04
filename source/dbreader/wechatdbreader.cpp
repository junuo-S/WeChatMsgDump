#include "wechatdbreader.h"

#include "defines.h"

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

void WechatDbReader::selectAllStrTalkerFromMSG(QObject* receiver, const char* method, const QVariantMap& param /* = QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select strTalker, count(strTalker) as total from MSG group by strTalker order by total desc;";
	data->dbThreadPool->executeQuery(sql, receiver, method, context);
}

void WechatDbReader::selectHeadImageByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select usrName, bigHeadImgUrl, smallHeadImgUrl from ContactHeadImgUrl where usrName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value(STR_USERNAME).toString()), receiver, method, context);
}

void WechatDbReader::selectContactByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select * from Contact where UserName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value(STR_USERNAME).toString()), receiver, method, context);
}

void WechatDbReader::selectAllSessionInfo(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql =
		"select Contact.Remark, Contact.NickName, Contact.Alias, MSG.strTalker, count(MSG.strTalker) as chatCount "
		"from MSG "
		"join Contact on MSG.strTalker = Contact.UserName "
		"group by strTalker "
		"order by chatCount desc;";
	data->dbThreadPool->executeQuery(sql, receiver, method, context);
}

void WechatDbReader::selectChatCountByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select count(*) as chatCount from MSG where strTalker = '%1'";
	data->dbThreadPool->executeQuery(sql.arg(param.value(STR_USERNAME).toString()), receiver, method, context);
}

void WechatDbReader::selectChatHistoryByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	QString sql = "select * from MSG "
		"where strTalker = '%1' "
		"and CreateTime %2 '%3' "
		"order by CreateTime %4 "
		"limit %5;";
	bool forward = param.value(STR_FORWARD).toBool();
	sql = sql.arg(param.value(STR_USERNAME).toString())
		.arg(forward ? ">=" : "<=")
		.arg(param.value(STR_CREATETIME).toULongLong())
		.arg(forward ? "asc" : "desc")
		.arg(param.value(STR_LIMIT).toUInt());
	data->dbThreadPool->executeQuery(sql, receiver, method, context);
}

