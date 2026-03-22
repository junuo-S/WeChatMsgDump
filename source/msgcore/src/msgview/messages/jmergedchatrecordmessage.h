#pragma once

#include "jmessagebase.h"

class JMergedChatRecordMessage : public JMessageBase
{
public:
    explicit JMergedChatRecordMessage(const QVariantMap& raw);
};
