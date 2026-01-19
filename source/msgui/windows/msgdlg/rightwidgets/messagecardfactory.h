#pragma once

#include <QVariantMap>

#include "global.h"
#include "dbparser/MSGParser.h"

class MessageCardWidgetBase;
class MessageCardWidgetFactoryItemBase;
class MessageCardWidgetFactory
{
public:
	static MessageCardWidgetBase* createInstance(const QVariantMap& msg, QWidget* parent);

private:
	static void registerItem(MessageCardWidgetFactoryItemBase* item);
	static void unregisterItem(MessageCardWidgetFactoryItemBase* item);
	friend class MessageCardWidgetFactoryItemBase;
};

class MessageCardWidgetFactoryItemBase
{
public:
	MessageCardWidgetFactoryItemBase(MsgType type);
	virtual ~MessageCardWidgetFactoryItemBase();
	virtual MessageCardWidgetBase* createInstance(const MSGParser& parser, QWidget* parent) = 0;
	MsgType m_type = MsgType::UnKnown;
};

template <class T>
class MessageCardWidgetFactoryItem : public MessageCardWidgetFactoryItemBase
{
public:
	MessageCardWidgetFactoryItem(MsgType type) : MessageCardWidgetFactoryItemBase(type) {}
	virtual MessageCardWidgetBase* createInstance(const MSGParser& parser, QWidget* parent) override
	{
		return new T(parser, parent);
	}
};

#define DECLAREFACTORY_MESSAGECARDWIDGET(type, Class) \
	MessageCardWidgetFactoryItem<Class> gs_##Class##_factory(type);
