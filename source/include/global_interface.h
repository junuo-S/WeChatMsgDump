#pragma once

#include <combaseapi.h>
#include <wrl/client.h>
#include <vector>
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
    Event_Combine
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

interface __declspec(uuid("AF3C6963-83BD-4237-ACE6-18A3E2639862"))
IJWeChatDBDecryptManager : public IJCoreSubject
{
    STDMETHOD_(bool, StartReadWeChatProcess)() PURE;
    STDMETHOD_(bool, StartDecryptDataBase)() PURE;
};

interface __declspec(uuid("62409794-5AA0-4A39-B514-8058B8FACFFE"))
IJCoreApplication : public IJUnknown
{
    STDMETHOD_(IJWeChatDBDecryptManager*, GetDecryptManager)() PURE;
};

interface __declspec(uuid("F410462B-D54A-4F50-BB3B-37C943DA412F"))
IJCoreEntry : public IJUnknown
{
    STDMETHOD_(IJCoreApplication*, GetCoreApplication)() PURE;
};