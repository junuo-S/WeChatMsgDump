#include "junknownmessage.h"

JUnknownMessage::JUnknownMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::UnKnown)
{
}
