#include "jmessagefactory.h"

#include <QHash>
#include <defines.h>

#include "junknownmessage.h"

static QHash<JMsgType, JMessageFactoryItemBase*>& FactoryCollection()
{
    static QHash<JMsgType, JMessageFactoryItemBase*> s_collection;
    return s_collection;
}

MessagePtr JMessageFactory::create(const QVariantMap& raw)
{
    const short type = static_cast<short>(raw.value(STR_TYPE).toInt());
    const short subType = static_cast<short>(raw.value(STR_SUBTYPE).toInt());
    const JMsgType key = resolveMsgType(type, subType);

    if (auto item = FactoryCollection().value(key); item)
        return item->create(raw);
    return std::make_shared<JUnknownMessage>(raw);
}

JMsgType JMessageFactory::resolveMsgType(short type, short subType)
{
    if (type == 1 && subType == 0)
        return JMsgType::PlantText;
    if (type == 3 && subType == 0)
        return JMsgType::Image;
    if (type == 34 && subType == 0)
        return JMsgType::Voice;
    if (type == 43 && subType == 0)
        return JMsgType::Video;
    if (type == 47 && subType == 0)
        return JMsgType::Sticker;
    if (type == 49 && subType == 5)
        return JMsgType::SharedCardLink;
    if (type == 49 && subType == 19)
        return JMsgType::MergedChatRecord;
    if (type == 49 && subType == 57)
        return JMsgType::TextWithQuote;
    if (type == 49 && subType == 2000)
        return JMsgType::Transfer;
    if (type == 49 && (subType == 0 || subType == 6))
        return JMsgType::File;
    if (type == 10000 && (subType == 0 || subType == 8000))
        return JMsgType::SystemNotice;
    if (type == 10000 && subType == 4)
        return JMsgType::Tickle;
    return JMsgType::UnKnown;
}

void JMessageFactory::registerItem(JMessageFactoryItemBase* item)
{
    if (!item)
        return;
    FactoryCollection().insert(item->m_msgType, item);
}

void JMessageFactory::unregisterItem(JMessageFactoryItemBase* item)
{
    if (!item)
        return;
    FactoryCollection().remove(item->m_msgType);
}

JMessageFactoryItemBase::JMessageFactoryItemBase(JMsgType msgType)
    : m_msgType(msgType)
{
    JMessageFactory::registerItem(this);
}

JMessageFactoryItemBase::~JMessageFactoryItemBase()
{
    JMessageFactory::unregisterItem(this);
}
