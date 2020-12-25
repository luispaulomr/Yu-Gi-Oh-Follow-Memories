#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <handleapi.h>
#include <Psapi.h>
#include <vector>
#include <string>
#include <iostream>

class CHandleProcess {

private:

	HANDLE m_hProcess = nullptr; 
	MODULEINFO m_modInfo = {0};
	SYSTEM_INFO m_sysInfo;
	HMODULE m_hModule = nullptr;
	std::wstring m_windowName = L"";
	std::wstring m_exeName = L"";
	std::vector<uint8_t> m_rawProcessMemory;

	std::string GetLastErrorAsString();

	HMODULE GetModule(HANDLE p_handle, std::wstring exe_name);

	uintptr_t _FindPattern(std::vector<uint8_t> data, uintptr_t base_address, const char* pattern, uintptr_t offset, uintptr_t count);

	uintptr_t _FindExternalPattern(HANDLE h_proc, const char* pattern, uintptr_t begin, intptr_t size);

public:

	CHandleProcess(const std::wstring& str_window_name, const std::wstring& str_exe_name);

	bool AttachToProcess();

	bool IsProcessRunning();

	uintptr_t FindCachedPattern(const char* pattern);

	uintptr_t FindPattern(const char* pattern);

	std::vector<uint8_t> ReadCachedMemory(uintptr_t addr, size_t size);

	std::vector<uint8_t> ReadMemory(uintptr_t addr, size_t size);

	void WriteMemory(uintptr_t addr, const std::vector<uint8_t>& to_write);

};

