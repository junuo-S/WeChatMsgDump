#pragma once

#include <combaseapi.h>
#include <wrl/client.h>
#include <algorithm>
#include <vector>
#include <atomic>
#include <string>

using Microsoft::WRL::ComPtr;

interface __declspec(uuid("0457DA00-BD7C-42FC-A554-FED3C2A7D6BE"))
IJUnknown : public IUnknown
{
    virtual ~IJUnknown() {}
};

enum class EventType : short
{
    Event_Unknown = 0xf0,
    Event_ProcessRead,
    Event_Decrypt,
    Event_Combine
};

struct IJCoreEvent
{
    STDMETHOD_(EventType, Type)();
};

struct JProcessReadEvent : public IJCoreEvent
{
    JProcessReadEvent(
        const unsigned long processId,
        const std::wstring& exeFilePath,
        const std::string& version,
        const std::wstring& nickName,
        const std::string& userName,
        const std::string& phoneNumber,
        const std::string& wxid,
        const std::wstring& dataPath);
    STDMETHODIMP_(EventType) Type() override;

    unsigned long m_processId;
    std::wstring m_exeFilePath;
    std::string m_version;
    std::wstring m_nickName;
    std::string m_userName;
    std::string m_phoneNumber;
    std::string m_wxid;
    std::wstring m_dataPath;
};

struct JDecryptEvent : public IJCoreEvent
{
    JDecryptEvent(const std::wstring& inputFile, const std::wstring& outputFile, const bool bSuc, int totalCount);
    STDMETHODIMP_(EventType) Type() override;

    std::wstring m_inputFile;
    std::wstring m_outputFile;
    bool m_bSuc;
    int m_totalCount;
};

struct JCombineEvent : public IJCoreEvent
{
    JCombineEvent(const std::wstring& currentFile, const std::wstring& finalFile, const bool bSuc, int totalCount);
	STDMETHODIMP_(EventType) Type() override;

    std::wstring m_currentFile;
    std::wstring m_finalFile;
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

interface __declspec(uuid("CB10F03C-110B-4926-9D3B-9FB69867CD45"))
IJCoreObserver : public IJUnknown
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