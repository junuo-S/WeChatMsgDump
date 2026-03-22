#include "jmessagebase.h"

#include <QObject>

#include <defines.h>

JMessageBase::JMessageBase(const QVariantMap& raw, JMsgType type)
    : m_rawRecord(raw)
    , m_msgType(type)
{
}

JMsgType JMessageBase::GetMsgType() const
{
    return m_msgType;
}

QString JMessageBase::GetStrTalker() const
{
    return m_rawRecord.value(STR_STRTALKER).toString();
}

qint64 JMessageBase::GetCreateTime() const
{
    return m_rawRecord.value(STR_CREATETIME).toLongLong();
}

bool JMessageBase::IsSender() const
{
    return m_rawRecord.value(STR_ISSENDER).toBool();
}

QString JMessageBase::GetDisplayText() const
{
    switch (m_msgType)
    {
    case JMsgType::Image:
        return QObject::tr("[Image]");
    case JMsgType::Voice:
        return QObject::tr("[Voice]");
    case JMsgType::Video:
        return QObject::tr("[Video]");
    case JMsgType::Sticker:
        return QObject::tr("[Sticker]");
    case JMsgType::File:
        return QObject::tr("[File]");
    case JMsgType::Transfer:
        return QObject::tr("[Transfer]");
    case JMsgType::MergedChatRecord:
        return QObject::tr("[Chat History]");
    case JMsgType::SystemNotice:
    case JMsgType::Tickle:
    case JMsgType::PlantText:
    case JMsgType::TextWithQuote:
    case JMsgType::SharedCardLink:
        return m_rawRecord.value(STR_STRCONTENT).toString().replace('\n', ' ');
    default:
        break;
    }
    return QObject::tr("[Unknown msg]");
}

QVariant JMessageBase::GetRawField(const QString& key) const
{
    return m_rawRecord.value(key);
}
