#include "jwechatdbdecryptorfactory.h"

#include <QHash>

static QHash<WeChatMajorVersion, JWeChatDBDecryptorFactoryItemBase*>& FactoryCollection()
{
	static QHash<WeChatMajorVersion, JWeChatDBDecryptorFactoryItemBase*> s_collection;
	return s_collection;
}

JWeChatDBDecryptorFactoryItemBase::JWeChatDBDecryptorFactoryItemBase(const WeChatMajorVersion majorVersion)
	: m_majorVersion(majorVersion)
{
	JWeChatDBDecryptorFactory::registerItem(this);
}

JWeChatDBDecryptorFactoryItemBase::~JWeChatDBDecryptorFactoryItemBase()
{
	JWeChatDBDecryptorFactory::unregisterItem(this);
}

JAbstractWeChatDBDecryptor* JWeChatDBDecryptorFactory::createInstance(const WeChatMajorVersion majorVersion)
{
	auto item = FactoryCollection().value(majorVersion);
	return item ? item->createInstance() : nullptr;
}

void JWeChatDBDecryptorFactory::registerItem(JWeChatDBDecryptorFactoryItemBase* item)
{
	if (!item)
		return;
	FactoryCollection().insert(item->m_majorVersion, item);
}

void JWeChatDBDecryptorFactory::unregisterItem(JWeChatDBDecryptorFactoryItemBase* item)
{
	if (!item)
		return;
	FactoryCollection().remove(item->m_majorVersion);
}
