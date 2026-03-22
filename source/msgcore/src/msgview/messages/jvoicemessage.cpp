#include "jvoicemessage.h"
#include "jmessagefactory.h"

JVoiceMessage::JVoiceMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::Voice)
{
}

DECLAREFACTORY_MESSAGE(Voice, JVoiceMessage)
