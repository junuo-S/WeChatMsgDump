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

class DBDECRYPTOR_EXPORT WxMemoryReader
{
public:
	~WxMemoryReader();
	std::string getWxPhoneNumber();
	std::wstring getWxUserName();
	std::string getWxNumber();
	std::string getSecretKey();
	BOOL reset();
	bool isSuccessFulRead() const;
	static WxMemoryReader* instance();

protected:
	WxMemoryReader();

private:
	BOOL readWxProcessMemory();
	BOOL readPhoneNumber(DWORD_PTR address);
	BOOL readUserName(DWORD_PTR address);
	BOOL readWxNumber(DWORD_PTR address);
	BOOL readSecretKey(DWORD_PTR address);
	void resetWxProcessInfo();
	std::string decToHex(size_t dec);
	DWORD_PTR byteArrayToAddress(BYTE* byte, size_t pointerLen);

	static WxMemoryReader* s_reader;
	DWORD m_processId = NULL;
	HANDLE m_hProcess = NULL;
	DWORD_PTR m_weChatDllAdress = NULL;
	std::string m_phoneNumber;
	std::wstring m_userName;
	std::string m_wxNumber;
	std::string m_secretKey;
	bool m_isSuccessed = false;
};
