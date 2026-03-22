#include "jimagemessage.h"
#include "jmessagefactory.h"

JImageMessage::JImageMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::Image)
{
}

DECLAREFACTORY_MESSAGE(Image, JImageMessage)
