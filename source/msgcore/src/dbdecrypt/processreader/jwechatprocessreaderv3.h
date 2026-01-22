#pragma once

#include <windows.h>

#include "jabstractwechatprocessreader.h"

class JWeChatProcessReaderV3 : public JAbstractWeChatProcessReader
{
public:
	JWeChatProcessReaderV3();
	virtual qulonglong GetWeChatProcessId() override;
	virtual QString GetWeChatExecutablePath() override;
	virtual QString GetWeChatVersion() override;
	virtual QString GetNickName() override;
	virtual QString GetWeChatUserName() override;
	virtual QString GetPhoneNumber() override;
	virtual QString GetWxid() override;
	virtual QString GetDataPath() override;
	virtual QString GetKey() override;
	virtual void Reset() override;

private:
	void initConfig();
	void patternScan();

	HANDLE m_hProcess = 0;
	DWORD m_processId = 0;
	QString m_exeFilePath;
	QString m_version;
	QString m_nickName;
	DWORD_PTR m_nickNameAddress = 0;
	QString m_userName;
	DWORD_PTR m_userNameAddress = 0;
	QString m_phoneNumber;
	DWORD_PTR m_phoneNumberAddress = 0;
	QString m_wxid;
	QString m_dataPath;
	QString m_key;
	DWORD_PTR m_keyAddress = 0;
	std::vector<DWORD_PTR> m_patternScanAddressList;

	static constexpr const wchar_t* const s_processName = L"WeChat.exe";
	static constexpr const wchar_t* const s_dllName = L"WeChatWin.dll";
};
