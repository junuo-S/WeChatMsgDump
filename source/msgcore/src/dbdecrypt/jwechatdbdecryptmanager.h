#pragma once

#include <global_interface.h>

class JWeChatDBDecryptManager : public IJWeChatDBDecryptManager
{
public:
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IJWeChatDBDecryptManager)
	END_INTERFACE_MAP()

	STDMETHODIMP_(bool) StartReadWeChatProcess() override;
	STDMETHODIMP_(bool) StartDecryptDataBase() override;
};