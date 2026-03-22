#include "messagecardfactory.h"

#include "messagecard.h"

static QHash<JMsgType, MessageCardWidgetFactoryItemBase*>& FactoryCollection()
{
	static QHash<JMsgType, MessageCardWidgetFactoryItemBase*> collection;
	return collection;
}

MessageCardWidgetBase* MessageCardWidgetFactory::createInstance(const MessagePtr& msg, QWidget* parent)
{
	if (!msg)
		return nullptr;
	if (FactoryCollection().contains(msg->GetMsgType()))
		return FactoryCollection().value(msg->GetMsgType())->createInstance(msg, parent);
	return new MessageCardUnKnownWidget(msg, parent);
}

void MessageCardWidgetFactory::registerItem(MessageCardWidgetFactoryItemBase* item)
{
	if (!FactoryCollection().contains(item->m_type))
		FactoryCollection().insert(item->m_type, item);
}

void MessageCardWidgetFactory::unregisterItem(MessageCardWidgetFactoryItemBase* item)
{
	FactoryCollection().remove(item->m_type);
}

MessageCardWidgetFactoryItemBase::MessageCardWidgetFactoryItemBase(JMsgType type)
	: m_type(type)
{
	MessageCardWidgetFactory::registerItem(this);
}

MessageCardWidgetFactoryItemBase::~MessageCardWidgetFactoryItemBase()
{
	MessageCardWidgetFactory::unregisterItem(this);
}
