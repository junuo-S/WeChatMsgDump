﻿#include "winprocessutils.h"

#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <winver.h>
#include <string>
#include <sstream>

static std::wstring TCHARToWString(const TCHAR* tchar)
{
	std::wstringstream wss;
	wss << tchar;
	return wss.str();
}

DWORD utils::GetProcessIdByName(const char* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
		return NULL;
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) 
	{
		if (strcmp(pe.szExeFile, pName) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return NULL;
}

DWORD_PTR utils::GetModuleAddress(const char* processName, const char* dllName)
{
	DWORD targetProcessId = GetProcessIdByName(processName);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
	if (hProcess == NULL)
		return NULL;

	HMODULE hModules[1024];
	DWORD cbNeeded;
	EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded);
	for (int i = 0; i < cbNeeded / sizeof(HMODULE); i++)
	{
		TCHAR szModuleName[MAX_PATH];
		GetModuleBaseName(hProcess, hModules[i], szModuleName, sizeof(szModuleName) / sizeof(TCHAR));
		if (_tcscmp(szModuleName, _T(dllName)) == 0)
		{
			DWORD_PTR baseAddress = (DWORD_PTR)hModules[i];
			return baseAddress;
		}
	}
	return NULL;
}

std::wstring utils::GetProcessName(DWORD processId)
{
	std::wstring processName;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (hProcess != NULL)
	{
		TCHAR buffer[MAX_PATH];
		if (GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH))
			processName = TCHARToWString(buffer);
		CloseHandle(hProcess);
	}
	return processName;
}

std::wstring utils::GetExecutablePath(DWORD processId)
{
	std::wstring executablePath;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (hProcess == NULL)
		return executablePath;
	HMODULE hModule;
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
	{
		WCHAR szProcessPath[MAX_PATH];
		if (GetModuleFileNameExW(hProcess, hModule, szProcessPath, MAX_PATH))
			executablePath = szProcessPath;
	}
	CloseHandle(hProcess);
	return executablePath;
}

std::string utils::GetFileVersion(const wchar_t* fileName)
{
	std::string version;
	VS_FIXEDFILEINFO* pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	int iVerInfoSize = GetFileVersionInfoSizeW(fileName, NULL);
	if (iVerInfoSize == 0)
		return version;
	char* pBuf = new char[iVerInfoSize];
	if (!pBuf)
		return version;
	if (GetFileVersionInfoW(fileName, 0, iVerInfoSize, pBuf))
	{
		if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize))
		{
			sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
			version = pBuf;
		}
	}
	delete []pBuf;
	return version;
}

std::wstring utils::GetMoudlePath()
{
	HMODULE hModule = GetModuleHandle(NULL);
	std::wstring path;
	if (hModule != NULL)
	{
		WCHAR szPath[MAX_PATH];
		DWORD length = GetModuleFileNameW(hModule, szPath, MAX_PATH);
		if (length)
			path = szPath;
	}
	int index = path.find_last_of('\\');
	if (index == -1)
		index = path.find_last_of('/');
	if (index == -1)
		return std::wstring();
	return path.substr(0, index);
}

std::wstring utils::GetExecutablePath(const char* processName)
{
	DWORD processId = GetProcessIdByName(processName);
	return GetExecutablePath(processId);
}

