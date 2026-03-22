#pragma once

#include <global_interface.h>

class MessageCardWidgetBase;
class MessageCardWidgetFactoryItemBase;
class MessageCardWidgetFactory
{
public:
	static MessageCardWidgetBase* createInstance(const MessagePtr& msg, QWidget* parent);

private:
	static void registerItem(MessageCardWidgetFactoryItemBase* item);
	static void unregisterItem(MessageCardWidgetFactoryItemBase* item);
	friend class MessageCardWidgetFactoryItemBase;
};

class MessageCardWidgetFactoryItemBase
{
public:
	MessageCardWidgetFactoryItemBase(JMsgType type);
	virtual ~MessageCardWidgetFactoryItemBase();
	virtual MessageCardWidgetBase* createInstance(const MessagePtr& message, QWidget* parent) = 0;
	JMsgType m_type = JMsgType::UnKnown;
};

template <class T>
class MessageCardWidgetFactoryItem : public MessageCardWidgetFactoryItemBase
{
public:
	MessageCardWidgetFactoryItem(JMsgType type) : MessageCardWidgetFactoryItemBase(type) {}
	virtual MessageCardWidgetBase* createInstance(const MessagePtr& message, QWidget* parent) override
	{
		return new T(message, parent);
	}
};

#define DECLAREFACTORY_MESSAGECARDWIDGET(type, Class) \
	MessageCardWidgetFactoryItem<Class> gs_##Class##_factory(type);
