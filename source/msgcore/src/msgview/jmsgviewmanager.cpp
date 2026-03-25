#include "jmsgviewmanager.h"

#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <QPointer>
#include <QVariantList>

#include <defines.h>

#include "../main.h"
#include "../dbdecrypt/jwechatdbdecryptmanager.h"
#include "../dbreader/wechatdbreader.h"
#include "icon/jonlineiconengine.h"
#include "messages/jmessagefactory.h"

static constexpr const char* const gs_strTalkerList = "talkerList";
static constexpr const char* const gs_strTalker = "talker";
static constexpr const char* const gs_strCount = "count";
static constexpr const char* const gs_strMessages = "messages";
static constexpr const char* const gs_strIsSessionLastQuery = "isSessionLastQuery";

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
    QVariantMap queryContext;
    queryContext.insert(gs_strTalker, talker);
    m_spDbReader->selectChatCountByUserName(this, "onSelectMessageCountFinished", { { STR_USERNAME, talker } }, queryContext);
    return true;
}

bool JMsgViewManager::StartQuerySessionLastMessage(const QString& talker)
{
    if (!ensureDbReader())
        return false;

    QVariantMap param;
    param.insert(STR_USERNAME, talker);
    param.insert(STR_FORWARD, false);
    param.insert(STR_CREATETIME, QDateTime::currentSecsSinceEpoch());
    param.insert(STR_LIMIT, 1U);

    QVariantMap queryContext;
    queryContext.insert(gs_strTalker, talker);
    queryContext.insert(gs_strIsSessionLastQuery, true);
    m_spDbReader->selectChatHistoryByUserName(this, "onSelectChatHistoryFinished", param, queryContext);
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

    QVariantMap queryContext;
    queryContext.insert(gs_strTalker, talker);
    queryContext.insert(gs_strIsSessionLastQuery, false);
    m_spDbReader->selectChatHistoryByUserName(this, "onSelectChatHistoryFinished", param, queryContext);
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
    data.insert(gs_strTalkerList, talkers);
    notifyMsgViewResult(MsgViewOpType::Op_QueryAllStrTalker, !talkers.isEmpty(), data);
}

void JMsgViewManager::onSelectMessageCountFinished(const QVariantList& result, const QVariant& context)
{
    const QVariantMap queryContext = context.toMap();
    QVariantMap data;
    data.insert(gs_strTalker, queryContext.value(gs_strTalker).toString());
    data.insert(gs_strCount, result.isEmpty() ? 0ULL : result.first().toMap().value(STR_CHATCOUNT).toULongLong());
    notifyMsgViewResult(MsgViewOpType::Op_QueryMessageCount, true, data);
}

void JMsgViewManager::onSelectChatHistoryFinished(const QVariantList& result, const QVariant& context)
{
    const QVariantMap queryContext = context.toMap();
    QVariantList messageList;
    messageList.reserve(result.size());
    for (const QVariant& row : result)
    {
        MessagePtr message = JMessageFactory::create(row.toMap());
        messageList.append(QVariant::fromValue(message));
    }

    QVariantMap data;
    data.insert(gs_strTalker, queryContext.value(gs_strTalker).toString());
    data.insert(gs_strMessages, messageList);
    if (queryContext.value(gs_strIsSessionLastQuery).toBool())
        notifyMsgViewResult(MsgViewOpType::Op_QuerySessionLastMessage, true, data);
    else
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
        {
            QPointer<JMsgViewManager> guard(this);
            JOnlineIconEngine* iconEngine = new JOnlineIconEngine(url);
            iconEngine->setReadyCallback([guard, wxid]()
                {
                    if (!guard)
                        return;
                    QVariantMap readyData;
                    readyData.insert(STR_WXID, wxid);
                    guard->notifyMsgViewResult(MsgViewOpType::Op_ContactHeadImageReady, true, readyData);
                });
            info.m_headImage = QIcon(iconEngine);
        }
    }

    m_contactCache.insert(wxid, new ContactInfo(info));

    if (!contactRecord)
        return;

    QVariantMap data;
    data.insert(STR_WXID, info.m_wxid);
    notifyMsgViewResult(MsgViewOpType::Op_QueryContactInfo, true, data);
}
