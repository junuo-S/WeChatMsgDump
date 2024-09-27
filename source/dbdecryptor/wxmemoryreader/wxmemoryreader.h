#pragma once

#ifndef DBDECRYPTOR_EXPORT

#ifdef _DB_DECRYPTOR_MODULE_
#define DBDECRYPTOR_EXPORT __declspec(dllexport)
#else
#define DBDECRYPTOR_EXPORT __declspec(dllimport)
#endif // _DB_DECRYPTOR_MODULE_

#endif // DBDECRYPTOR_EXPORT

#include <windows.h>
#include <string>
#include <vector>

class DBDECRYPTOR_EXPORT WxMemoryReader
{
public:
	~WxMemoryReader();
	std::string getWxVersion() const;
	std::wstring getWxExePath() const;
	std::string getWxPhoneNumber() const;
	std::wstring getWxUserName() const;
	std::string getWxNumber() const;
	std::string getSecretKey() const;
	std::string getWxid() const;
	std::wstring getWxDataPath() const;
	BOOL reRead();
	bool isSuccessFulRead() const;
	DWORD getWxProcessId() const;
	static WxMemoryReader* instance();

protected:
	WxMemoryReader();

private:
	BOOL readWxProcessMemory();
	BOOL readPhoneNumber(DWORD_PTR address);
	BOOL readUserName(DWORD_PTR address);
	BOOL readWxNumber(DWORD_PTR address);
	BOOL readSecretKey(DWORD_PTR address);
	BOOL readWxid();
	BOOL readWxDataPath();
	BOOL patternScanForAddress();
	void resetWxProcessInfo();
	std::string decToHex(size_t dec) const;
	bool isWxidFormat(const std::string& wxid) const;
	DWORD_PTR byteArrayToAddress(BYTE* byte, size_t pointerLen);

	static WxMemoryReader* s_reader;
	DWORD m_processId = NULL;
	HANDLE m_hProcess = NULL;
	DWORD_PTR m_weChatDllAdress = NULL;
	std::string m_wxVersion;
	std::wstring m_wxExePath;
	std::string m_phoneNumber;
	std::wstring m_userName;
	std::string m_wxNumber;
	std::string m_secretKey;
	std::string m_wxid;
	std::wstring m_wxDataPath;
	std::vector<DWORD_PTR> m_patternScanAddressRet;
	bool m_isSuccessed = false;
};
