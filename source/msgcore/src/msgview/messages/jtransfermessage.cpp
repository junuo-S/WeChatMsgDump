#include "jtransfermessage.h"
#include "jmessagefactory.h"

JTransferMessage::JTransferMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::Transfer)
{
}

DECLAREFACTORY_MESSAGE(Transfer, JTransferMessage)
