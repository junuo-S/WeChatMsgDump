#pragma once

#include <memory>
#include <QString>

#include <global_interface.h>
#include <junuobase/junuocombase.h>

enum class WeChatMajorVersion : short
{
	Version_UnKnown = -1,
	Version_3 = 3,
	Version_4 = 4,
};

class JAbstractWeChatProcessReader;
class JWeChatDBDecryptManager : public IJWeChatDBDecryptManager
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJWeChatDBDecryptManager)
	END_INTERFACE_MAP()

	JWeChatDBDecryptManager();

	STDMETHODIMP_(bool) StartReadWeChatProcess() override;
	STDMETHODIMP_(bool) StartDecryptDataBase() override;
	QString getFinalDBFileName() const;

private:
	WeChatMajorVersion m_majorVersion = WeChatMajorVersion::Version_UnKnown;
	std::shared_ptr<JAbstractWeChatProcessReader> m_processReader;
	QString m_finalDBFileName;
};