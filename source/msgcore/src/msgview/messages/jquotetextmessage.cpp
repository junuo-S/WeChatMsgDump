#include "jquotetextmessage.h"
#include "jmessagefactory.h"

JQuoteTextMessage::JQuoteTextMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::TextWithQuote)
{
}

DECLAREFACTORY_MESSAGE(TextWithQuote, JQuoteTextMessage)
