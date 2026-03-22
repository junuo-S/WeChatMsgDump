#pragma once

#include "jmessagebase.h"

class JVoiceMessage : public JMessageBase
{
public:
    explicit JVoiceMessage(const QVariantMap& raw);
};
