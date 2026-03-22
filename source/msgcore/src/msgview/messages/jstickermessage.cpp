#include "jstickermessage.h"
#include "jmessagefactory.h"

JStickerMessage::JStickerMessage(const QVariantMap& raw)
    : JMessageBase(raw, JMsgType::Sticker)
{
}

DECLAREFACTORY_MESSAGE(Sticker, JStickerMessage)
