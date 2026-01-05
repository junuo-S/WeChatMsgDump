#pragma once

#include <atomic>

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

template<class T>
class JunuoComAgent : public T
{
public:
    virtual ULONG STDMETHODCALLTYPE AddRef() override
    {
        return ++m_ref;
    }

    virtual ULONG STDMETHODCALLTYPE Release() override
    {
		ULONG ref = --m_ref;
		if (ref == 0) delete this;
		return ref;
    }

    static T* Create()
    {
        return new JunuoComAgent;
    }

private:
    std::atomic_ulong m_ref = 0;
};

#define JUNUO_COM_NEW(TypeName) JunuoComAgent<TypeName>::Create()
