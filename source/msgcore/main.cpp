#include "dbdecrypt/jwechatdbdecryptmanager.h"

#include <junuobase/junuocombase.h>

class JCoreApplication : public IJCoreApplication
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJCoreApplication)
	END_INTERFACE_MAP()

	JCoreApplication() : m_spDecryptMgr(JUNUO_COM_NEW(JWeChatDBDecryptManager))
	{
	}

	STDMETHODIMP_(IJWeChatDBDecryptManager*) GetDecryptManager() override
	{
		return m_spDecryptMgr.Get();
	}

private:
	ComPtr<IJWeChatDBDecryptManager> m_spDecryptMgr;
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
