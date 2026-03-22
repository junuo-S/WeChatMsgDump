#pragma once

#include "jmessagebase.h"

class JImageMessage : public JMessageBase
{
public:
    explicit JImageMessage(const QVariantMap& raw);
};
