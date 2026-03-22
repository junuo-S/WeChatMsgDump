#pragma once

#include "jmessagebase.h"

class JStickerMessage : public JMessageBase
{
public:
    explicit JStickerMessage(const QVariantMap& raw);
};
