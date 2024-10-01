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

void WechatDbReader::selectAllStrTalkerFromMSG(QObject* receiver, const char* method, const QVariantMap& param /* = QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select distinct(strTalker) from MSG;";
	data->dbThreadPool->executeQuery(sql, receiver, method, context);
}

void WechatDbReader::selectHeadImageByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select usrName, bigHeadImgUrl, smallHeadImgUrl from ContactHeadImgUrl where usrName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value("userName").toString()), receiver, method, context);
}

void WechatDbReader::selectContactByUserName(QObject* receiver, const char* method, const QVariantMap& param /*= QVariantMap()*/, const QVariant& context /*= QVariant()*/)
{
	const QString sql = "select UserName, Alias, NickName, Remark from Contact where UserName = '%1';";
	data->dbThreadPool->executeQuery(sql.arg(param.value("userName").toString()), receiver, method, context);
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

