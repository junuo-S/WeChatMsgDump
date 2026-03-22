#pragma once

#include "jmessagebase.h"

class JSystemNoticeMessage : public JMessageBase
{
public:
    explicit JSystemNoticeMessage(const QVariantMap& raw);
};
