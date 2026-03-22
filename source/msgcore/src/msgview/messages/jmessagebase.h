#pragma once

#include <global_interface.h>

class JMessageBase : public IJMessage
{
public:
    JMessageBase(const QVariantMap& raw, JMsgType type);
    ~JMessageBase() override = default;

    JMsgType GetMsgType() const override;
    QString GetStrTalker() const override;
    qint64 GetCreateTime() const override;
    bool IsSender() const override;
    QString GetDisplayText() const override;
    QVariant GetRawField(const QString& key) const override;

protected:
    QVariantMap m_rawRecord;
    JMsgType m_msgType = JMsgType::UnKnown;
};
