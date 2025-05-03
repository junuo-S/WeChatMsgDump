#include "jwechatprocessreaderfactory.h"

#include <QHash>

static QHash<WeChatMajorVersion, JWeChatProcessReaderFactoryItemBase*>& FactoryCollection()
{
	static QHash<WeChatMajorVersion, JWeChatProcessReaderFactoryItemBase*> s_collection;
	return s_collection;
}

JWeChatProcessReaderFactoryItemBase::JWeChatProcessReaderFactoryItemBase(const WeChatMajorVersion majorVersion)
	: m_majorVersion(majorVersion)
{
	JWeChatProcessReaderFactory::registerItem(this);
}

JWeChatProcessReaderFactoryItemBase::~JWeChatProcessReaderFactoryItemBase()
{
	JWeChatProcessReaderFactory::unregisterItem(this);
}

JAbstractWeChatProcessReader* JWeChatProcessReaderFactory::createInstance(const WeChatMajorVersion majorVersion)
{
	auto item = FactoryCollection().value(majorVersion);
	return item ? item->createInstance() : nullptr;
}

void JWeChatProcessReaderFactory::registerItem(JWeChatProcessReaderFactoryItemBase* item)
{
	if (!item)
		return;
	FactoryCollection().insert(item->m_majorVersion, item);
}

void JWeChatProcessReaderFactory::unregisterItem(JWeChatProcessReaderFactoryItemBase* item)
{
	if (!item)
		return;
	FactoryCollection().remove(item->m_majorVersion);
}
