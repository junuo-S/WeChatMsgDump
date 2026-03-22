#pragma once

#include "jmessagebase.h"

class JTextMessage : public JMessageBase
{
public:
    explicit JTextMessage(const QVariantMap& raw);
};
