#include "jlinkmessage.h"
#include "jmessagefactory.h"

JLinkMessage::JLinkMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::SharedCardLink)
{
}

DECLAREFACTORY_MESSAGE(SharedCardLink, JLinkMessage)
