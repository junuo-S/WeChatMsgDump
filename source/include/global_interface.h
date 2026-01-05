#pragma once

#include <combaseapi.h>
#include <wrl/client.h>
#include <vector>
#include <QString>

using Microsoft::WRL::ComPtr;

interface __declspec(uuid("0457DA00-BD7C-42FC-A554-FED3C2A7D6BE"))
IJUnknown : public IUnknown
{
    virtual ~IJUnknown() {}
};

enum class EventType : short
{
    Event_Unknown = 0xf0,
    Event_ProcessReadFinished,
    Event_Decrypt,
    Event_Combine
};

struct IJCoreEvent
{
    STDMETHOD_(EventType, Type)();
};

struct JProcessReadFinishedEvent : public IJCoreEvent
{
    JProcessReadFinishedEvent(
        const unsigned long processId,
        const QString& exeFilePath,
        const QString& version,
        const QString& nickName,
        const QString& userName,
        const QString& phoneNumber,
        const QString& wxid,
        const QString& dataPath);
    STDMETHODIMP_(EventType) Type() override;

    unsigned long m_processId;
    QString m_exeFilePath;
    QString m_version;
    QString m_nickName;
    QString m_userName;
    QString m_phoneNumber;
    QString m_wxid;
    QString m_dataPath;
};

struct JDecryptEvent : public IJCoreEvent
{
    JDecryptEvent(const QString& inputFile, const QString& outputFile, const bool bSuc, int totalCount);
    STDMETHODIMP_(EventType) Type() override;

    QString m_inputFile;
    QString m_outputFile;
    bool m_bSuc;
    int m_totalCount;
};

struct JCombineEvent : public IJCoreEvent
{
    JCombineEvent(const QString& currentFile, const QString& finalFile, const bool bSuc, int totalCount);
	STDMETHODIMP_(EventType) Type() override;

    QString m_currentFile;
    QString m_finalFile;
    bool m_bSuc;
    int m_totalCount;
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