#include "jfilemessage.h"
#include "jmessagefactory.h"

JFileMessage::JFileMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::File)
{
}

DECLAREFACTORY_MESSAGE(File, JFileMessage)
