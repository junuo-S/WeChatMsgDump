#include "wxmemoryreader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "winprocessutils/winprocessutils.h"

static const char* const gs_wxProcessName = "WeChat.exe";
static const char* const gs_wxDllName = "WeChatWin.dll";

BOOL WxMemoryReader::readWxProcessMemory()
{
	if (m_hProcess == NULL)
		return FALSE;
	std::string wxVersion = utils::GetFileVersion(utils::GetExecutablePath(gs_wxProcessName).c_str());
	if (wxVersion.empty())
		return FALSE;
	QFile jsonFile(QString::fromStdWString(utils::GetMoudlePath()) + "/../cfgs/versionconfig.json");
	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return FALSE;
	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
	jsonData.clear();
	if (jsonDoc.isNull() || !jsonDoc.isObject())
		return FALSE;
	QJsonObject versionInfo = jsonDoc.object().value(wxVersion.c_str()).toObject();
	if (versionInfo.isEmpty())
		return FALSE;

	if (!readPhoneNumber(m_weChatDllAdress + versionInfo.value("phonenumber").toInt()))
		return FALSE;

	if (!readUserName(m_weChatDllAdress + versionInfo.value("username").toInt()))
		return FALSE;

	if (!readWxNumber(m_weChatDllAdress + versionInfo.value("wxnumber").toInt()))
		return FALSE;
	
	if (!readSecretKey(m_weChatDllAdress + versionInfo.value("secretkey").toInt()))
		return FALSE;
	
	return TRUE;
}

BOOL WxMemoryReader::readPhoneNumber(DWORD_PTR address)
{
	BYTE phoneNumberBuffer[24] = { 0 };
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), phoneNumberBuffer, sizeof(phoneNumberBuffer), NULL))
		return FALSE;
	m_phoneNumber = std::string(reinterpret_cast<const char*>(phoneNumberBuffer));
	return TRUE;
}

BOOL WxMemoryReader::readUserName(DWORD_PTR address)
{
	const size_t pointerLen = sizeof(void*);
	BYTE* userNamePointerBuffer = new BYTE[pointerLen];
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), userNamePointerBuffer, pointerLen, NULL))
		return FALSE;
	DWORD_PTR userNameAddress = byteArrayToAddress(userNamePointerBuffer, pointerLen);
	delete[]userNamePointerBuffer;
	userNamePointerBuffer = nullptr;
	BYTE userNameBuffer[1024] = { 0 };
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(userNameAddress), userNameBuffer, sizeof(userNameBuffer), NULL))
		return FALSE;
	int byteLength = sizeof(userNameBuffer);
	int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(userNameBuffer), byteLength, nullptr, 0);
	wchar_t* wideCharArray = new wchar_t[wideCharLength + 1];
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(userNameBuffer), byteLength, wideCharArray, wideCharLength);
	wideCharArray[wideCharLength] = L'\0';
	m_userName = wideCharArray;
	delete[] wideCharArray;
	return TRUE;
}

BOOL WxMemoryReader::readWxNumber(DWORD_PTR address)
{
	BYTE wxNumberBuffer[64] = { 0 };
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), wxNumberBuffer, sizeof(wxNumberBuffer), NULL))
		return FALSE;
	m_wxNumber = std::string(reinterpret_cast<const char*>(wxNumberBuffer));
	return TRUE;
}

BOOL WxMemoryReader::readSecretKey(DWORD_PTR address)
{
	const size_t pointerLen = sizeof(void*);
	BYTE* keyPointerBuffer = new BYTE[pointerLen];
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), keyPointerBuffer, pointerLen, NULL))
		return FALSE;
	DWORD_PTR keyAddress = byteArrayToAddress(keyPointerBuffer, pointerLen);
	delete[]keyPointerBuffer;
	keyPointerBuffer = nullptr;
	BYTE keyBuffer[64];
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(keyAddress), keyBuffer, sizeof(keyBuffer), NULL))
		return FALSE;
	for (size_t index = 0; keyBuffer[index] != '\0' && index < sizeof(keyBuffer); index++)
		m_secretKey += decToHex(keyBuffer[index]);
	return TRUE;
}

void WxMemoryReader::resetWxProcessInfo()
{
	m_processId = utils::GetProcessIdByName(gs_wxProcessName);
	m_hProcess = OpenProcess(PROCESS_VM_READ, FALSE, m_processId);
	m_weChatDllAdress = utils::GetModuleAddress(gs_wxProcessName, gs_wxDllName);
}

std::string WxMemoryReader::decToHex(size_t dec)
{
	static std::string hexBase = "0123456789ABCDEF";
	std::string result;
	while (dec != 0)
	{
		unsigned int temp = dec % 16;
		result.insert(result.begin(), hexBase[temp]);
		dec /= 16;
	}
	return result;
}

DWORD_PTR WxMemoryReader::byteArrayToAddress(BYTE* byte, size_t pointerLen)
{
	DWORD_PTR address = 0;
	for (size_t index = 0; index < pointerLen; index++)
	{
		long long temp = byte[pointerLen - 1 - index];
		address |= temp << pointerLen * (pointerLen - 1 - index);
	}
	return address;
}

WxMemoryReader* WxMemoryReader::s_reader = new WxMemoryReader;

WxMemoryReader::~WxMemoryReader()
{
	CloseHandle(m_hProcess);
}

std::string WxMemoryReader::getWxPhoneNumber()
{
	return m_phoneNumber;
}

std::wstring WxMemoryReader::getWxUserName()
{
	return m_userName;
}

std::string WxMemoryReader::getWxNumber()
{
	return m_wxNumber;
}

std::string WxMemoryReader::getSecretKey()
{
	return m_secretKey;
}

BOOL WxMemoryReader::reset()
{
	resetWxProcessInfo();
	m_isSuccessed = readWxProcessMemory();
	return m_isSuccessed;
}

WxMemoryReader* WxMemoryReader::instance()
{
	return s_reader;
}

WxMemoryReader::WxMemoryReader()
{
	reset();
}

