#pragma once

#include "jmessagebase.h"

class JLinkMessage : public JMessageBase
{
public:
    explicit JLinkMessage(const QVariantMap& raw);
};
