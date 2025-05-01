#include "dbdecrypt/jwechatdbdecryptmanager.h"

class JCoreApplication : public IJCoreApplication
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJCoreApplication)
	END_INTERFACE_MAP()

	JCoreApplication()
	{
		m_spDecryptMgr.Attach(new JWeChatDBDecryptManager);
	}

	STDMETHODIMP_(IJWeChatDBDecryptManager*) GetDecryptManager()
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

	JCoreEntry()
	{
		m_spApp.Attach(new JCoreApplication);
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
	static JCoreEntry entry;
	return &entry;
}
