#include "wxmemoryreader.h"

#include <Python.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <comdef.h>
#include <algorithm>
#include <sys/stat.h>

#include "utils/winprocessutils.h"

static const char* const gs_wxProcessName = "WeChat.exe";
static const char* const gs_wxDllName = "WeChatWin.dll";

BOOL WxMemoryReader::readWxProcessMemory()
{
	if (m_hProcess == NULL)
		return FALSE;
	m_wxVersion = utils::GetFileVersion(utils::GetExecutablePath(gs_wxProcessName).c_str());
	if (m_wxVersion.empty())
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
	QJsonObject versionInfo = jsonDoc.object().value(m_wxVersion.c_str()).toObject();
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
	
	return patternScanForAddress() && readWxid() && readWxDataPath();
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
	if (!utils::IsAddressInProcess(m_hProcess, reinterpret_cast<LPCVOID>(userNameAddress)))
		userNameAddress = address;
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

BOOL WxMemoryReader::readWxid()
{
	m_wxids.clear();
	for (auto cit = m_patternScanAddressRet.cbegin(); cit != m_patternScanAddressRet.cend(); cit++)
	{
		BYTE buffer[80];
		if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(*cit-40), buffer, sizeof(buffer), NULL))
			continue;
		QString bufferString = reinterpret_cast<const char*>(buffer);
		QStringList splitTemp = bufferString.split("\\Msg");
		std::string wxid = splitTemp.first().split("\\").last().toStdString();
		if (isWxidFormat(wxid) && std::find(m_wxids.cbegin(), m_wxids.cend(), wxid) == m_wxids.cend())
			m_wxids.push_back(wxid);
	}
	return !m_wxids.empty();
}

BOOL WxMemoryReader::readWxDataPath()
{
	m_wxDataPaths.clear();
	static BYTE rootTarget[] = {':', '\\'};
	static BYTE endTarget[] = {'\\', 'M', 's', 'g'};
	for (auto cit = m_patternScanAddressRet.cbegin(); cit != m_patternScanAddressRet.cend(); cit++)
	{
		BYTE buffer[256];
		if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(*cit - 128), buffer, sizeof(buffer), NULL))
			continue;
		auto rootPos = std::search(buffer, buffer + sizeof(buffer), rootTarget, rootTarget + sizeof(rootTarget));
		auto endPos = std::search(buffer, buffer + sizeof(buffer), endTarget, endTarget + sizeof(endTarget));
		int byteLength = endPos - rootPos + 1;
		int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(rootPos - 1), byteLength, nullptr, 0);
		wchar_t* wideCharArray = new wchar_t[wideCharLength + 1];
		MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(rootPos - 1), byteLength, wideCharArray, wideCharLength);
		wideCharArray[wideCharLength] = L'\0';
		std::wstring path = wideCharArray;
		delete[] wideCharArray;
		struct _stat __stat;
		if (_wstat(path.c_str(), &__stat) == 0 && std::find(m_wxDataPaths.cbegin(), m_wxDataPaths.cend(), path) == m_wxDataPaths.cend())
			m_wxDataPaths.push_back(path);
	}
	return !m_wxDataPaths.empty();
}

BOOL WxMemoryReader::patternScanForAddress()
{
	static std::string patternStr = "\\\\Msg\\\\FTSContact";
	static size_t patternStrLength = patternStr.length();
	static DWORD_PTR maxAddress = utils::IsWx64BitExecutable(m_wxExePath = utils::GetExecutablePath(gs_wxProcessName)) ? 0x7FFFFFFF0000 : 0x7fff0000;
	m_patternScanAddressRet.clear();
	Py_Initialize();
	_bstr_t currentPath = utils::GetMoudlePath().c_str();
	PyObject* sysPath = PySys_GetObject("path");
	PyList_Append(sysPath, PyUnicode_FromString(currentPath));
	PyObject* pModule = PyImport_ImportModule("pymemutils");
	if (!pModule)
	{
		PyErr_Print();
		Py_Finalize();
		return FALSE;
	}
	PyObject* pFunc = PyObject_GetAttrString(pModule, "patternScanAllAddress");
	if (!pFunc || !PyCallable_Check(pFunc))
	{
		PyErr_Print();
		Py_DECREF(pModule);
		Py_Finalize();
		return FALSE;
	}
	PyObject* pArgs = PyTuple_Pack(4,
		PyLong_FromLongLong(m_processId),
		PyUnicode_FromString(patternStr.c_str()),
		PyLong_FromLongLong(maxAddress),
		PyLong_FromLong(100)
	);
	PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
	if (!pValue)
	{
		PyErr_Print();
	}
	else
	{
		Py_ssize_t size = PyList_Size(pValue);
		for (Py_ssize_t i = 0; i < size; ++i)
		{
			PyObject* item = PyList_GetItem(pValue, i);
			if (item && PyLong_Check(item))
			{
				DWORD_PTR value = PyLong_AsLongLong(item);
				m_patternScanAddressRet.push_back(value);
			}
		}
		Py_DECREF(pValue);
	}
	Py_XDECREF(pArgs);
	Py_XDECREF(pFunc);
	Py_XDECREF(pModule);
	Py_Finalize();
	return !m_patternScanAddressRet.empty();
}

void WxMemoryReader::resetWxProcessInfo()
{
	m_processId = utils::GetProcessIdByName(gs_wxProcessName);
	m_hProcess = OpenProcess(PROCESS_VM_READ, FALSE, m_processId);
	m_weChatDllAdress = utils::GetModuleAddress(gs_wxProcessName, gs_wxDllName);
}

std::string WxMemoryReader::decToHex(size_t dec) const
{
	static std::string hexBase = "0123456789ABCDEF";
	std::string result;
	while (dec != 0)
	{
		unsigned int temp = dec % 16;
		result.insert(result.begin(), hexBase[temp]);
		dec /= 16;
	}
	while (result.length() < 2)
		result.insert(result.begin(), '0');
	return result;
}

bool WxMemoryReader::isWxidFormat(const std::string& wxid) const
{
	if (wxid.length() < 5 || wxid.substr(0, 5).compare("wxid_") != 0)
		return false;
	for (size_t i = 5; i < wxid.length(); i++)
	{
		char ch = wxid.at(i);
		bool isNumber = ch >= '0' && ch <= '9';
		bool isEnglish = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
		bool isUnderline = ch == '_';
		if (!isNumber && !isEnglish && !isUnderline)
			return false;
	}
	return true;
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

std::string WxMemoryReader::getWxVersion() const
{
	return m_wxVersion;
}

std::wstring WxMemoryReader::getWxExePath() const
{
	return m_wxExePath;
}

std::string WxMemoryReader::getWxPhoneNumber() const
{
	return m_phoneNumber;
}

std::wstring WxMemoryReader::getWxUserName() const
{
	return m_userName;
}

std::string WxMemoryReader::getWxNumber() const
{
	return m_wxNumber;
}

std::string WxMemoryReader::getSecretKey() const
{
	return m_secretKey;
}

std::vector<std::string> WxMemoryReader::getWxids() const
{
	return m_wxids;
}

std::vector<std::wstring> WxMemoryReader::getWxDataPaths() const
{
	return m_wxDataPaths;
}

BOOL WxMemoryReader::reset()
{
	resetWxProcessInfo();
	m_isSuccessed = readWxProcessMemory();
	return m_isSuccessed;
}

bool WxMemoryReader::isSuccessFulRead() const
{
	return m_isSuccessed;
}

DWORD WxMemoryReader::getWxProcessId() const
{
	return m_processId;
}

WxMemoryReader* WxMemoryReader::instance()
{
	return s_reader;
}

WxMemoryReader::WxMemoryReader()
{
	
}

