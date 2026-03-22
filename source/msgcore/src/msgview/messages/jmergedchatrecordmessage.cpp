#include "jmergedchatrecordmessage.h"
#include "jmessagefactory.h"

JMergedChatRecordMessage::JMergedChatRecordMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::MergedChatRecord)
{
}

DECLAREFACTORY_MESSAGE(MergedChatRecord, JMergedChatRecordMessage)
