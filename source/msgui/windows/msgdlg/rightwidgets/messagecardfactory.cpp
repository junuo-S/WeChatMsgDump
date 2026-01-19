#include "messagecardfactory.h"

#include "messagecard.h"

static QHash<MsgType, MessageCardWidgetFactoryItemBase*>& FactoryCollection()
{
	static QHash<MsgType, MessageCardWidgetFactoryItemBase*> collection;
	return collection;
}

MessageCardWidgetBase* MessageCardWidgetFactory::createInstance(const QVariantMap& msg, QWidget* parent)
{
	MSGParser parser(msg);
	if (FactoryCollection().contains(parser.getMsgType()))
		return FactoryCollection().value(parser.getMsgType())->createInstance(parser, parent);
	return new MessageCardUnKnownWidget(parser, parent);
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

MessageCardWidgetFactoryItemBase::MessageCardWidgetFactoryItemBase(MsgType type)
	: m_type(type)
{
	MessageCardWidgetFactory::registerItem(this);
}

MessageCardWidgetFactoryItemBase::~MessageCardWidgetFactoryItemBase()
{
	MessageCardWidgetFactory::unregisterItem(this);
}
