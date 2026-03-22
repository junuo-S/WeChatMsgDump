#include "jvideomessage.h"
#include "jmessagefactory.h"

JVideoMessage::JVideoMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::Video)
{
}

DECLAREFACTORY_MESSAGE(Video, JVideoMessage)
