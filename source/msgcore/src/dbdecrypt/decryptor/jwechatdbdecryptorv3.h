#pragma once

#include "jabstractwechatdbdecryptor.h"
#include "jwechatdbdecryptorfactory.h"

class JWeChatDBDecryptorV3 : public JAbstractWeChatDBDecryptor
{
public:
	virtual bool Decrypt() override;
};
