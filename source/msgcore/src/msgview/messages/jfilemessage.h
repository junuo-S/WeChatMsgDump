#pragma once

#include "jmessagebase.h"

class JFileMessage : public JMessageBase
{
public:
    explicit JFileMessage(const QVariantMap& raw);
};
