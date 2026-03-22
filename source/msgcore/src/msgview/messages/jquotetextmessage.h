#pragma once

#include "jmessagebase.h"

class JQuoteTextMessage : public JMessageBase
{
public:
    explicit JQuoteTextMessage(const QVariantMap& raw);
};
