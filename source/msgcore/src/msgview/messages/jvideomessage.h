#pragma once

#include "jmessagebase.h"

class JVideoMessage : public JMessageBase
{
public:
    explicit JVideoMessage(const QVariantMap& raw);
};
