#pragma once

#include <QString>

class JAbstractWeChatProcessReader
{
public:
	virtual unsigned long GetWeChatProcessId() = 0;
	virtual QString GetWeChatExecutablePath() = 0;
	virtual QString GetWeChatVersion() = 0;
	virtual QString GetNickName() = 0;
	virtual QString GetWeChatUserName() = 0;
	virtual QString GetPhoneNumber() = 0;
	virtual QString GetWxid() = 0;
	virtual QString GetDataPath() = 0;
	virtual QString GetKey() = 0;
	virtual void Reset() = 0;
};
