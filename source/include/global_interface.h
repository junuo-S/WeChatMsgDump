#pragma once

#include <combaseapi.h>
#include <wrl/client.h>
#include <algorithm>
#include <vector>
#include <atomic>

using Microsoft::WRL::ComPtr;

#define BEGIN_INTERFACE_MAP() \
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override { \
        if (!ppvObject) return E_POINTER; \
        *ppvObject = nullptr;

#define INTERFACE_ENTRY(InterfaceType) \
    if (riid == __uuidof(InterfaceType)) { \
        *ppvObject = static_cast<InterfaceType*>(this); \
        AddRef(); \
        return S_OK; \
    }

#define END_INTERFACE_MAP() \
    if (riid == __uuidof(IUnknown)) { \
        *ppvObject = static_cast<IUnknown*>(this); \
        AddRef(); \
        return S_OK; \
    } \
    return E_NOINTERFACE; \
    }


interface __declspec(uuid("0457DA00-BD7C-42FC-A554-FED3C2A7D6BE"))
IJUnknown : public IUnknown
{
    virtual ~IJUnknown() {}
    virtual ULONG STDMETHODCALLTYPE AddRef() override;
    virtual ULONG STDMETHODCALLTYPE Release() override;

protected:
    std::atomic<ULONG> m_ref = 1;
};

struct IJCoreEvent
{
    STDMETHOD_(int, Type)() PURE;
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