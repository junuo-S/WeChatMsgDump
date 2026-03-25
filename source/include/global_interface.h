#pragma once

#include <combaseapi.h>
#include <wrl/client.h>
#include <memory>
#include <optional>
#include <vector>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QVariantMap>

using Microsoft::WRL::ComPtr;

interface __declspec(uuid("0457DA00-BD7C-42FC-A554-FED3C2A7D6BE"))
IJUnknown : public IUnknown
{
    virtual ~IJUnknown() {}
};

enum class EventType : short
{
    Event_Unknown = 0xf0,
    Event_ReadProcess,
    Event_Decrypt,
    Event_Combine,
    Event_MsgView
};

enum class MsgViewOpType : short
{
    Op_Unknown = 0,
    Op_QueryAllStrTalker,
    Op_QueryMessageCount,
    Op_QuerySessionLastMessage,
    Op_QueryChatHistory,
    Op_QueryContactInfo,
    Op_ContactHeadImageReady
};

enum class JMsgType : short
{
    UnKnown = -1,
    PlantText = 0,
    Image,
    Voice,
    Video,
    Sticker,
    SharedCardLink,
    MergedChatRecord,
    TextWithQuote,
    Transfer,
    File,
    SystemNotice,
    Tickle,
};

struct ContactInfo
{
    QString m_wxid;
    QString m_userName;
    QString m_nickName;
    QString m_remarkName;
    QIcon m_headImage;
};

struct IJCoreEvent
{
    STDMETHOD_(EventType, Type)();
	EventType m_type = EventType::Event_Unknown;
};

struct JCommonAsyncEvent : public IJCoreEvent
{
    enum class SubType : short
    {
		SubType_Begin = 0x0,
        SubType_Progress,
		SubType_End
    };

	SubType m_subType = SubType::SubType_Begin;
	float m_progress = 0.f;
	QString m_trMessage;
	QVariantMap m_extraData;
};

struct JMsgViewAsyncEvent : public JCommonAsyncEvent
{
    MsgViewOpType m_op = MsgViewOpType::Op_Unknown;
    bool m_ok = false;
};

interface IJCoreObserver;
interface __declspec(uuid("2195FF53-607D-4CF1-913E-44F356F6EF97"))
IJCoreSubject : public IJUnknown
{
    STDMETHOD_(void, AttachObserver)(IJCoreObserver * observer);
    STDMETHOD_(void, DetachObserver)(IJCoreObserver* observer);
    STDMETHOD_(void, Notify)(IJCoreEvent* event);

private:
	std::vector<IJCoreObserver*> m_observers;
};

interface IJCoreObserver
{
    virtual ~IJCoreObserver();
    STDMETHOD_(bool, OnCoreEvent)(IJCoreEvent* event) PURE;

protected:
    void attachTo(IJCoreSubject* subject);
    void detachFrom(IJCoreSubject* subject);

private:
    std::vector<IJCoreSubject*> m_subjects;
};

interface IJMessage
{
    virtual ~IJMessage() {}
    virtual JMsgType GetMsgType() const = 0;
    virtual QString GetStrTalker() const = 0;
    virtual qint64 GetCreateTime() const = 0;
    virtual bool IsSender() const = 0;
    virtual QString GetDisplayText() const = 0;
    virtual QVariant GetRawField(const QString& key) const = 0;
};

using MessagePtr = std::shared_ptr<IJMessage>;

interface __declspec(uuid("AF3C6963-83BD-4237-ACE6-18A3E2639862"))
IJWeChatDBDecryptManager : public IJCoreSubject
{
    STDMETHOD_(bool, StartReadWeChatProcess)() PURE;
    STDMETHOD_(bool, StartDecryptDataBase)() PURE;
};

interface __declspec(uuid("E2D0D469-7D8E-4576-9A2B-D2B1A5578EE7"))
IJMsgViewManager : public IJCoreSubject
{
    STDMETHOD_(std::optional<ContactInfo>, GetContactInfo)(const QString& wxid) PURE;
    STDMETHOD_(QString, GetSelfWxid)() PURE;
    STDMETHOD_(bool, StartQueryAllStrTalker)() PURE;
    STDMETHOD_(bool, StartQueryMessageCount)(const QString& talker) PURE;
    STDMETHOD_(bool, StartQuerySessionLastMessage)(const QString& talker) PURE;
    STDMETHOD_(bool, StartQueryChatHistory)(const QString& talker, quint64 ts, bool forward, uint limit) PURE;
};

interface __declspec(uuid("62409794-5AA0-4A39-B514-8058B8FACFFE"))
IJCoreApplication : public IJUnknown
{
    STDMETHOD_(IJWeChatDBDecryptManager*, GetDecryptManager)() PURE;
    STDMETHOD_(IJMsgViewManager*, GetMsgViewManager)() PURE;
};

interface __declspec(uuid("F410462B-D54A-4F50-BB3B-37C943DA412F"))
IJCoreEntry : public IJUnknown
{
    STDMETHOD_(IJCoreApplication*, GetCoreApplication)() PURE;
};

Q_DECLARE_METATYPE(ContactInfo)
Q_DECLARE_METATYPE(MessagePtr)