#pragma once

#include "jmessagebase.h"

class JUnknownMessage : public JMessageBase
{
public:
    explicit JUnknownMessage(const QVariantMap& raw);
};
