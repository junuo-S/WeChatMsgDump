#include "jmsgviewmanager.h"

#include <QDir>
#include <QFileInfo>
#include <QVariantList>

#include <defines.h>

#include "../main.h"
#include "../dbdecrypt/jwechatdbdecryptmanager.h"
#include "../dbreader/wechatdbreader.h"
#include "icon/jonlineiconengine.h"
#include "messages/jmessagefactory.h"

namespace
{
constexpr auto KEY_TALKER_LIST = "talkerList";
constexpr auto KEY_TALKER = "talker";
constexpr auto KEY_COUNT = "count";
constexpr auto KEY_MESSAGES = "messages";
}

JMsgViewManager::JMsgViewManager()
    : QObject(nullptr)
    , m_contactCache(1024)
{
    qRegisterMetaType<MessagePtr>("MessagePtr");
}

JMsgViewManager::~JMsgViewManager()
{
}

std::optional<ContactInfo> JMsgViewManager::GetContactInfo(const QString& wxid)
{
    if (ContactInfo* cached = m_contactCache.object(wxid))
        return *cached;

    if (!ensureDbReader())
        return std::nullopt;

    QVariantMap context;
    context.insert(STR_WXID, wxid);
    m_spDbReader->selectContactByUserName(this, "onSelectContactInfoFinished", { { STR_USERNAME, wxid } }, context);
    return std::nullopt;
}

QString JMsgViewManager::GetSelfWxid()
{
    return m_selfWxid;
}

bool JMsgViewManager::StartQueryAllStrTalker()
{
    if (!ensureDbReader())
        return false;
    m_spDbReader->selectAllStrTalkerFromMSG(this, "onSelectAllStrTalkerFinished");
    return true;
}

bool JMsgViewManager::StartQueryMessageCount(const QString& talker)
{
    if (!ensureDbReader())
        return false;
    QVariantMap ctx;
    ctx.insert(KEY_TALKER, talker);
    m_spDbReader->selectChatCountByUserName(this, "onSelectMessageCountFinished", { { STR_USERNAME, talker } }, ctx);
    return true;
}

bool JMsgViewManager::StartQueryChatHistory(const QString& talker, quint64 ts, bool forward, uint limit)
{
    if (!ensureDbReader())
        return false;

    QVariantMap param;
    param.insert(STR_USERNAME, talker);
    param.insert(STR_FORWARD, forward);
    param.insert(STR_CREATETIME, ts);
    param.insert(STR_LIMIT, limit);

    QVariantMap ctx;
    ctx.insert(KEY_TALKER, talker);
    m_spDbReader->selectChatHistoryByUserName(this, "onSelectChatHistoryFinished", param, ctx);
    return true;
}

void JMsgViewManager::onSelectAllStrTalkerFinished(const QVariantList& result, const QVariant& context)
{
    Q_UNUSED(context)
    QStringList talkers;
    talkers.reserve(result.size());
    for (const QVariant& row : result)
    {
        const QVariantMap rowMap = row.toMap();
        QString talker = rowMap.value(STR_STRTALKER).toString();
        if (!talker.isEmpty())
            talkers.append(talker);
    }

    QVariantMap data;
    data.insert(KEY_TALKER_LIST, talkers);
    notifyMsgViewResult(MsgViewOpType::Op_QueryAllStrTalker, !talkers.isEmpty(), data);
}

void JMsgViewManager::onSelectMessageCountFinished(const QVariantList& result, const QVariant& context)
{
    const QVariantMap ctx = context.toMap();
    QVariantMap data;
    data.insert(KEY_TALKER, ctx.value(KEY_TALKER).toString());
    data.insert(KEY_COUNT, result.isEmpty() ? 0ULL : result.first().toMap().value(STR_CHATCOUNT).toULongLong());
    notifyMsgViewResult(MsgViewOpType::Op_QueryMessageCount, true, data);
}

void JMsgViewManager::onSelectChatHistoryFinished(const QVariantList& result, const QVariant& context)
{
    const QVariantMap ctx = context.toMap();
    QVariantList messageList;
    messageList.reserve(result.size());
    for (const QVariant& row : result)
    {
        MessagePtr message = JMessageFactory::create(row.toMap());
        messageList.append(QVariant::fromValue(message));
    }

    QVariantMap data;
    data.insert(KEY_TALKER, ctx.value(KEY_TALKER).toString());
    data.insert(KEY_MESSAGES, messageList);
    notifyMsgViewResult(MsgViewOpType::Op_QueryChatHistory, true, data);
}

void JMsgViewManager::onSelectContactInfoFinished(const QVariantList& result, const QVariant& context)
{
    const QString wxid = context.toMap().value(STR_WXID).toString();
    if (result.isEmpty())
    {
        updateAndNotifyContact(wxid, nullptr);
        return;
    }
    const QVariantMap record = result.first().toMap();
    updateAndNotifyContact(wxid, &record);
}

bool JMsgViewManager::ensureDbReader()
{
    if (m_spDbReader)
        return true;

    IJCoreApplication* coreApp = getCoreApplication();
    IJWeChatDBDecryptManager* decryptMgr = coreApp ? coreApp->GetDecryptManager() : nullptr;
    auto* decryptImpl = dynamic_cast<JWeChatDBDecryptManager*>(decryptMgr);
    if (!decryptImpl)
        return false;

    const QString finalDb = decryptImpl->getFinalDBFileName();
    if (finalDb.isEmpty() || !QFileInfo::exists(finalDb))
        return false;

    QFileInfo finalDbInfo(finalDb);
    m_selfWxid = finalDbInfo.dir().dirName();
    m_spDbReader = new WechatDbReader(finalDb, this);
    return true;
}

void JMsgViewManager::notifyMsgViewResult(MsgViewOpType op, bool ok, const QVariantMap& data)
{
    JMsgViewAsyncEvent event;
    event.m_type = EventType::Event_MsgView;
    event.m_subType = JCommonAsyncEvent::SubType::SubType_End;
    event.m_op = op;
    event.m_ok = ok;
    event.m_extraData = data;
    Notify(&event);
}

void JMsgViewManager::updateAndNotifyContact(const QString& wxid, const QVariantMap* contactRecord)
{
    if (wxid.isEmpty())
        return;

    ContactInfo info;
    if (ContactInfo* cached = m_contactCache.object(wxid))
        info = *cached;

    info.m_wxid = wxid;
    info.m_userName = wxid;

    if (contactRecord)
    {
        info.m_userName = contactRecord->value("UserName", wxid).toString();
        info.m_nickName = contactRecord->value(STR_NICKNAME).toString();
        info.m_remarkName = contactRecord->value(STR_REMARK).toString();
    }

    if (contactRecord)
    {
        const QString bigUrl = contactRecord->value("bigHeadImgUrl").toString();
        const QString smallUrl = contactRecord->value("smallHeadImgUrl").toString();
        const QString url = !bigUrl.isEmpty() ? bigUrl : smallUrl;
        if (!url.isEmpty())
            info.m_headImage = QIcon(new JOnlineIconEngine(url));
    }

    m_contactCache.insert(wxid, new ContactInfo(info));

    if (!contactRecord)
        return;

    QVariantMap data;
    data.insert(STR_WXID, info.m_wxid);
    notifyMsgViewResult(MsgViewOpType::Op_QueryContactInfo, true, data);
}
