#pragma once

#include <QCache>
#include <QObject>

#include <global_interface.h>
#include <junuobase/junuocombase.h>

class WechatDbReader;

class JMsgViewManager : public QObject, public IJMsgViewManager
{
    Q_OBJECT
public:
    BEGIN_INTERFACE_MAP()
        INTERFACE_ENTRY(IJMsgViewManager)
    END_INTERFACE_MAP()

    JMsgViewManager();
    ~JMsgViewManager() override;

    STDMETHODIMP_(std::optional<ContactInfo>) GetContactInfo(const QString& wxid) override;
    STDMETHODIMP_(QString) GetSelfWxid() override;
    STDMETHODIMP_(bool) StartQueryAllStrTalker() override;
    STDMETHODIMP_(bool) StartQueryMessageCount(const QString& talker) override;
    STDMETHODIMP_(bool) StartQuerySessionLastMessage(const QString& talker) override;
    STDMETHODIMP_(bool) StartQueryChatHistory(const QString& talker, quint64 ts, bool forward, uint limit) override;

private:
    Q_INVOKABLE void onSelectAllStrTalkerFinished(const QVariantList& result, const QVariant& context = QVariant());
    Q_INVOKABLE void onSelectMessageCountFinished(const QVariantList& result, const QVariant& context = QVariant());
    Q_INVOKABLE void onSelectChatHistoryFinished(const QVariantList& result, const QVariant& context = QVariant());
    Q_INVOKABLE void onSelectContactInfoFinished(const QVariantList& result, const QVariant& context = QVariant());

    bool ensureDbReader();
    void notifyMsgViewResult(MsgViewOpType op, bool ok, const QVariantMap& data = QVariantMap());
    void updateAndNotifyContact(const QString& wxid, const QVariantMap* contactRecord);

    WechatDbReader* m_spDbReader = nullptr;
    QCache<QString, ContactInfo> m_contactCache;
    QString m_selfWxid;
};
