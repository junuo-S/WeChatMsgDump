#include "jwechatprocessreaderv3.h"

#include <array>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <junuobase/utils/junuobaseutils.h>

#include "jwechatprocessreaderfactory.h"
#include "dbdecrypt/jwechatdbdecryptmanager.h"

JWeChatProcessReaderV3::JWeChatProcessReaderV3()
{
	initConfig();
}

JWeChatProcessReaderV3::~JWeChatProcessReaderV3()
{
	if (m_hProcess)
	{
		CloseHandle(m_hProcess);
		m_hProcess = 0;
	}
}

qulonglong JWeChatProcessReaderV3::GetWeChatProcessId()
{
	return m_processId;
}

QString JWeChatProcessReaderV3::GetWeChatExecutablePath()
{
	return m_exeFilePath;
}

QString JWeChatProcessReaderV3::GetWeChatVersion()
{
	return m_version;
}

QString JWeChatProcessReaderV3::GetNickName()
{
	if (!m_nickName.isEmpty())
		return m_nickName;
	static const size_t s_pointerLen = sizeof(void*);
	std::array<BYTE, s_pointerLen> nickNamePointerBuffer{};
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(m_nickNameAddress), nickNamePointerBuffer.data(), s_pointerLen, NULL))
		return QString();
	DWORD_PTR nickNameAddress = junuobase::utils::ByteArrayToAddress(nickNamePointerBuffer.data(), s_pointerLen);

	BYTE nickNameBuffer[1024] = { 0 };
	SIZE_T bytesRead = 0;
	// 首次试错，用户名这里不一定是指针
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(nickNameAddress), nickNameBuffer, sizeof(nickNameBuffer), &bytesRead))
		nickNameAddress = m_nickNameAddress;

	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(nickNameAddress), nickNameBuffer, sizeof(nickNameBuffer), &bytesRead))
		return QString();
	auto endIt = std::find(nickNameBuffer, nickNameBuffer + bytesRead, static_cast<BYTE>(0));
	int validLen = static_cast<int>(std::distance(nickNameBuffer, endIt));
	m_nickName = QString::fromUtf8(reinterpret_cast<const char*>(nickNameBuffer), validLen);
	return m_nickName;
}

QString JWeChatProcessReaderV3::GetWeChatUserName()
{
	if (!m_userName.isEmpty())
		return m_userName;
	BYTE userNameBuf[64] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(m_userNameAddress), userNameBuf, sizeof(userNameBuf), &bytesRead))
		return QString();
	auto endIt = std::find(userNameBuf, userNameBuf + bytesRead, static_cast<BYTE>(0));
	int validLen = static_cast<int>(std::distance(userNameBuf, endIt));
	m_userName = QString::fromUtf8(reinterpret_cast<const char*>(userNameBuf), validLen);
	return m_userName;
}

QString JWeChatProcessReaderV3::GetPhoneNumber()
{
	if (!m_phoneNumber.isEmpty())
		return m_phoneNumber;
	BYTE phoneNumberBuffer[24] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(m_phoneNumberAddress), phoneNumberBuffer, sizeof(phoneNumberBuffer), &bytesRead))
		return QString();
	auto endIt = std::find(phoneNumberBuffer, phoneNumberBuffer + bytesRead, static_cast<BYTE>(0));
	int validLen = static_cast<int>(std::distance(phoneNumberBuffer, endIt));
	m_phoneNumber = QString::fromUtf8(reinterpret_cast<const char*>(phoneNumberBuffer), validLen);
	return m_phoneNumber;
}

QString JWeChatProcessReaderV3::GetWxid()
{
	if (!m_wxid.isEmpty())
		return m_wxid;
	if (m_patternScanAddressList.empty())
		patternScan();

	static auto isWxidFormat = [](const QString& wxid)
		{
			if (wxid.length() < 5 || wxid.mid(0, 5).compare("wxid_") != 0)
				return false;
			for (size_t i = 5; i < wxid.length(); i++)
			{
				QChar ch = wxid.at(i);
				bool isNumber = ch >= '0' && ch <= '9';
				bool isEnglish = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
				bool isUnderline = ch == '_';
				if (!isNumber && !isEnglish && !isUnderline)
					return false;
			}
			return true;
		};

	for (auto cit = m_patternScanAddressList.cbegin(); cit != m_patternScanAddressList.cend(); cit++)
	{
		BYTE buffer[80] = { 0 };
		SIZE_T bytesRead = 0;
		if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(*cit - 40), buffer, sizeof(buffer), &bytesRead) || bytesRead == 0)
			continue;
		auto endIt = std::find(buffer, buffer + bytesRead, static_cast<BYTE>(0));
		int validLen = static_cast<int>(std::distance(buffer, endIt));
		QString bufferString = QString::fromUtf8(reinterpret_cast<const char*>(buffer), validLen);
		QStringList splitTemp = bufferString.split("\\Msg");
		QString wxid = splitTemp.first().split("\\").last();
		if (isWxidFormat(wxid))
		{
			m_wxid = wxid;
			break;
		}
	}
	return m_wxid;
}

QString JWeChatProcessReaderV3::GetDataPath()
{
	if (!m_dataPath.isEmpty())
		return m_dataPath;
	if (m_patternScanAddressList.empty())
		patternScan();
	static constexpr const char* const s_rootTarget = ":\\";
	static constexpr const char* const s_endTarget = "\\Msg";
	for (auto cit = m_patternScanAddressList.cbegin(); cit != m_patternScanAddressList.cend(); cit++)
	{
		BYTE buffer[256] = { 0 };
		SIZE_T bytesRead = 0;
		if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(*cit - 129), buffer, sizeof(buffer), &bytesRead) || bytesRead == 0)
			continue;
		auto endIt = std::find(buffer, buffer + bytesRead, static_cast<BYTE>(0));
		int validLen = static_cast<int>(std::distance(buffer, endIt));
		QString pathStr = QString::fromUtf8(reinterpret_cast<const char*>(buffer), validLen);
		int rootPos = pathStr.indexOf(s_rootTarget);
		int endPos = pathStr.indexOf(s_endTarget);
		if (rootPos <= 0 || endPos < rootPos)
			continue;
		QString path = pathStr.mid(rootPos - 1, endPos - rootPos + 1);
		if (QFile::exists(path))
		{
			m_dataPath = path;
			break;
		}
	}
	return m_dataPath;
}

QString JWeChatProcessReaderV3::GetKey()
{
	if (!m_key.isEmpty())
		return m_key;
	static const size_t s_pointerLen = sizeof(void*);
	std::array<BYTE, s_pointerLen> keyPointerBuffer{};
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(m_keyAddress), keyPointerBuffer.data(), s_pointerLen, NULL))
		return QString();
	DWORD_PTR keyAddress = junuobase::utils::ByteArrayToAddress(keyPointerBuffer.data(), s_pointerLen);

	BYTE keyBuffer[64];
	if (!ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(keyAddress), keyBuffer, sizeof(keyBuffer), NULL))
		return QString();
	for (size_t index = 0; keyBuffer[index] != '\0' && index < sizeof(keyBuffer); index++)
		m_key += junuobase::utils::DecToHex(keyBuffer[index]);
	return m_key;
}

void JWeChatProcessReaderV3::Reset()
{
	if (m_hProcess)
	{
		CloseHandle(m_hProcess);
		m_hProcess = 0;
	}
	m_processId = 0;
	m_exeFilePath.clear();
	m_version.clear();
	m_nickName.clear();
	m_nickNameAddress = 0;
	m_userName.clear();
	m_userNameAddress = 0;
	m_phoneNumber.clear();
	m_phoneNumberAddress = 0;
	m_wxid.clear();
	m_dataPath.clear();
	m_key.clear();
	m_keyAddress = 0;
	m_patternScanAddressList.clear();
	initConfig();
}

void JWeChatProcessReaderV3::initConfig()
{
	if (m_hProcess)
	{
		CloseHandle(m_hProcess);
		m_hProcess = 0;
	}

	m_processId = junuobase::utils::GetProcessIdByName(s_processName);
	m_hProcess = OpenProcess(PROCESS_VM_READ, FALSE, m_processId);
	DWORD_PTR dllAddress = junuobase::utils::GetModuleAddress(s_processName, s_dllName);
	m_exeFilePath = QString::fromStdWString(junuobase::utils::GetExecutablePath(m_processId));
	m_version = QString::fromStdString(junuobase::utils::GetFileVersion(m_exeFilePath.toStdWString().c_str()));
	QFile jsonFile(":/configs/offsetv3.json");
	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
	jsonData.clear();
	if (jsonDoc.isNull() || !jsonDoc.isObject())
		return;
	QJsonObject versionInfo = jsonDoc.object().value(m_version.toStdString().c_str()).toObject();
	if (versionInfo.isEmpty())
		return;
	m_nickNameAddress = dllAddress + versionInfo.value("nickname").toInteger();
	m_userNameAddress = dllAddress + versionInfo.value("username").toInteger();
	m_phoneNumberAddress = dllAddress + versionInfo.value("phonenumber").toInteger();
	m_keyAddress = dllAddress + versionInfo.value("secretkey").toInteger();
}

void JWeChatProcessReaderV3::patternScan()
{
	static std::string patternStr = "\\Msg\\FTSContact";
	static size_t patternStrLength = patternStr.length();
	static DWORD_PTR maxAddress = junuobase::utils::IsWx64BitExecutable(m_exeFilePath.toStdWString()) ? 0x7FFFFFFF0000 : 0x7fff0000;
	m_patternScanAddressList = junuobase::utils::PatternScanAllAddress(m_processId, patternStr, maxAddress, 100);
}

DECLAREFACTORY_PROCESS_READER(WeChatMajorVersion::Version_3, JWeChatProcessReaderV3)
