#include "jtextmessage.h"
#include "jmessagefactory.h"

JTextMessage::JTextMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::PlantText)
{
}

DECLAREFACTORY_MESSAGE(PlantText, JTextMessage)
