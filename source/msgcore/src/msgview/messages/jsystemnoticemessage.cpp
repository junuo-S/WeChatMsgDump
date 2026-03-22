#include "jsystemnoticemessage.h"
#include "jmessagefactory.h"

JSystemNoticeMessage::JSystemNoticeMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::SystemNotice)
{
}

DECLAREFACTORY_MESSAGE(SystemNotice, JSystemNoticeMessage)
DECLAREFACTORY_MESSAGE(Tickle, JSystemNoticeMessage)
