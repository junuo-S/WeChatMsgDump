#include "main.h"
#include "dbdecrypt/jwechatdbdecryptmanager.h"
#include "msgview/jmsgviewmanager.h"

#include <junuobase/junuocombase.h>

static IJCoreApplication* gs_coreApplication = nullptr;

IJCoreApplication* getCoreApplication()
{
	return gs_coreApplication;
}

class JCoreApplication : public IJCoreApplication
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJCoreApplication)
	END_INTERFACE_MAP()

	JCoreApplication()
		: m_spDecryptMgr(JUNUO_COM_NEW(JWeChatDBDecryptManager))
		, m_spMsgViewMgr(JUNUO_COM_NEW(JMsgViewManager))
	{
		gs_coreApplication = this;
	}

	~JCoreApplication()
	{
		if (gs_coreApplication == this)
			gs_coreApplication = nullptr;
	}

	STDMETHODIMP_(IJWeChatDBDecryptManager*) GetDecryptManager() override
	{
		return m_spDecryptMgr.Get();
	}

	STDMETHODIMP_(IJMsgViewManager*) GetMsgViewManager() override
	{
		return m_spMsgViewMgr.Get();
	}

private:
	ComPtr<IJWeChatDBDecryptManager> m_spDecryptMgr;
	ComPtr<IJMsgViewManager> m_spMsgViewMgr;
};

class JCoreEntry : public IJCoreEntry
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJCoreEntry)
	END_INTERFACE_MAP()

	JCoreEntry() : m_spApp(JUNUO_COM_NEW(JCoreApplication))
	{
	}

	STDMETHODIMP_(IJCoreApplication*) GetCoreApplication() override
	{
		return m_spApp.Get();
	}

private:
	ComPtr<IJCoreApplication> m_spApp;
};

IJCoreEntry* GetCoreEntry()
{
	static ComPtr<JCoreEntry> entry = JUNUO_COM_NEW(JCoreEntry);
	return entry.Get();
}
