﻿#pragma once

#include <windows.h>
#include <string>

namespace utils
{
	DWORD GetProcessIdByName(const char* pName);
	DWORD_PTR GetModuleAddress(const char* processName, const char* dllName);
	std::wstring GetProcessName(DWORD processId);
	std::wstring GetExecutablePath(DWORD processId);
	std::wstring GetExecutablePath(const char* processName);
	std::string GetFileVersion(const wchar_t* fileName);
	std::wstring GetMoudlePath();
}
