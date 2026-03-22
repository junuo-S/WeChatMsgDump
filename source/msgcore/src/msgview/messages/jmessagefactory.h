#pragma once

#include <global_interface.h>

class JMessageFactoryItemBase;
class JMessageFactory
{
public:
    static MessagePtr create(const QVariantMap& raw);

private:
    static JMsgType resolveMsgType(short type, short subType);
    static void registerItem(JMessageFactoryItemBase* item);
    static void unregisterItem(JMessageFactoryItemBase* item);
    friend class JMessageFactoryItemBase;
};

class JMessageFactoryItemBase
{
public:
    explicit JMessageFactoryItemBase(JMsgType msgType);
    virtual ~JMessageFactoryItemBase();
    virtual MessagePtr create(const QVariantMap& raw) = 0;
    JMsgType m_msgType = JMsgType::UnKnown;
};

template <class T>
class JMessageFactoryItem : public JMessageFactoryItemBase
{
public:
    explicit JMessageFactoryItem(JMsgType msgType) : JMessageFactoryItemBase(msgType) {}
    MessagePtr create(const QVariantMap& raw) override
    {
        return std::make_shared<T>(raw);
    }
};

#define DECLAREFACTORY_MESSAGE(msgType, Class) \
    JMessageFactoryItem<Class> gs_##Class##_##msgType##_factory(JMsgType::msgType);
