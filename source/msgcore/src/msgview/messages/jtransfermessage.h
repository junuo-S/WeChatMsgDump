#pragma once

#include "jmessagebase.h"

class JTransferMessage : public JMessageBase
{
public:
    explicit JTransferMessage(const QVariantMap& raw);
};
