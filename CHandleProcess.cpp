#include "CHandleProcess.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

CHandleProcess::CHandleProcess(const std::wstring& str_window_name, const std::wstring& str_exe_name)
{
	m_hProcess = nullptr;
	m_windowName = str_window_name;
        m_exeName = str_exe_name;
}

bool CHandleProcess::AttachToProcess()
{
	HWND window_handle = FindWindowW(nullptr, m_windowName.c_str());

        if (!window_handle) {
                std::cout << "[CHandleProcess::AttachToProcess] In call to FindWindow, could not find window name\n";
                return false;
        }

        DWORD PID;
	GetWindowThreadProcessId(window_handle, &PID);

	m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, NULL, PID);

        if (!m_hProcess) {
                std::cout << "[CHandleProcess::AttachToProcess] ERROR when calling OpenProcess: " << GetLastErrorAsString() << "\n";
                return false;
        }

        m_hModule = GetModule(m_hProcess, m_exeName);

        if (!::GetModuleInformation(m_hProcess, m_hModule, &m_modInfo, sizeof(m_modInfo))) {
                std::cout << "[CHandleProcess::AttachToProcess] ERROR when calling GetModuleInformation: " << GetLastErrorAsString() << "\n";
                return false;
        }

        SYSTEM_INFO m_sysInfo;
        GetSystemInfo(&m_sysInfo);

        return true;
}

bool CHandleProcess::IsProcessRunning()
{
        if (!m_hProcess) {
                std::cout << "[CHandleProcess::IsProcessRunning] ERROR: Process has not been attached" << "\n";
                return false;
        }

        DWORD exitCode = 0;

        if (!GetExitCodeProcess(m_hProcess, &exitCode)) {
                std::cout << "[CHandleProcess::IsProcessRunning] ERROR: Unable to get process status" << "\n";
        }

        return exitCode == STILL_ACTIVE;
}

uintptr_t CHandleProcess::FindCachedPattern(const char* pattern)
{
        static bool firstCall = true;

        if (firstCall) {
                SIZE_T bytes_read;
                firstCall = false;
                m_rawProcessMemory.resize(1000000);
                ReadProcessMemory(m_hProcess, m_modInfo.lpBaseOfDll, m_rawProcessMemory.data(), m_rawProcessMemory.size(), &bytes_read);

                if (!bytes_read) {
                        std::cout << "[CHandleProcess::FindPattern] ERROR: Unable to read from process memory" << "\n";
                        return 0;
                }
        }

        return _FindPattern(std::ref(m_rawProcessMemory), (uintptr_t) m_modInfo.lpBaseOfDll, pattern, 0, 0);
}

uintptr_t CHandleProcess::FindPattern(const char* pattern)
{
        uintptr_t addr_size = 
                reinterpret_cast<uintptr_t>(m_sysInfo.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(m_sysInfo.lpMinimumApplicationAddress);

        return _FindExternalPattern(m_hProcess, pattern, reinterpret_cast<uintptr_t>(m_sysInfo.lpMinimumApplicationAddress), addr_size);
}

std::vector<uint8_t> CHandleProcess::ReadCachedMemory(uintptr_t addr, size_t size)
{
        std::vector<uint8_t> ret(size);
        size_t i_begin = addr - (size_t) m_modInfo.lpBaseOfDll;
        size_t i_end = i_begin + size;

        std::copy(m_rawProcessMemory.begin() + i_begin, m_rawProcessMemory.begin() + i_end, ret.begin());

        return ret;
}

std::vector<uint8_t> CHandleProcess::ReadMemory(uintptr_t addr, size_t size)
{
        std::vector<uint8_t> ret(size);
        SIZE_T bytes_read = 0;

        ReadProcessMemory(m_hProcess, (LPCVOID) addr, ret.data(), ret.size(), &bytes_read);

        if (bytes_read != size) {
                std::cout << "[CHandleProcess::ReadMemory] ERROR: Could not read the requested memory" << "\n";
                std::cout << "[CHandleProcess::ReadMemory] ERROR: address: " << std::hex << addr << std::dec << "\n";
                std::cout << "[CHandleProcess::ReadMemory] ERROR: bytes_read: " << bytes_read << "\n";
                return ret;
        }

        return ret;
}

void CHandleProcess::WriteMemory(uintptr_t addr, const std::vector<uint8_t>& to_write)
{
        SIZE_T bytes_written;

        WriteProcessMemory(m_hProcess, (LPVOID)addr, to_write.data(), to_write.size(), &bytes_written);

        if (bytes_written != to_write.size()) {
                std::cout << "[CHandleProcess::ReadMemory] ERROR: Could not write to the requested memory" << "\n";
                return;
        }
}

std::string CHandleProcess::GetLastErrorAsString()
{
        DWORD errorMessageID = ::GetLastError();

	if (!errorMessageID) {

		return std::string();

	}

        LPSTR messageBuffer = nullptr;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);

        LocalFree(messageBuffer);

        return message;
}

HMODULE CHandleProcess::GetModule(HANDLE p_handle, std::wstring exe_name)
{
        HMODULE hMods[1024];
        DWORD cbNeeded;
        unsigned int i;

        if (EnumProcessModules(p_handle, hMods, sizeof(hMods), &cbNeeded)) {
                for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {

                        TCHAR szModName[MAX_PATH];
                        if (GetModuleFileNameEx(p_handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                                std::wstring wstrModName = szModName;

                                if (wstrModName.find(exe_name) != std::string::npos) {
                                        return hMods[i];
                                }
                        }

                }
        }

        return nullptr;
}

/* Credits to user at0mos: https://forum.cheatengine.org/viewtopic.php?p=5588505 */
uintptr_t CHandleProcess::_FindPattern(std::vector<uint8_t> data, uintptr_t base_address, const char* pattern, uintptr_t offset, uintptr_t count)
{
        // Ensure the incoming pattern is properly aligned..
        if (strlen(pattern) % 2 > 0) {
                return 0;
        }

        // Convert the pattern to a vector of data..
        std::vector<std::pair<uint8_t, bool>> vpattern;

        for (size_t x = 0, y = strlen(pattern) / 2; x < y; x++) {
                // Obtain the current byte..
                std::stringstream stream(std::string(pattern + (x * 2), 2));

                // Check if this is a wildcard..
                if (stream.str() == "??") {
                        vpattern.push_back(std::make_pair(00, false));
                } else {
                        auto byte = strtol(stream.str().c_str(), nullptr, 16);
                        vpattern.push_back(std::make_pair((uint8_t)byte, true));
                }
        }

        auto scanStart = data.begin();
        auto result = (uintptr_t) 0;

        while (true) {
                // Search for the pattern..
                auto ret = std::search(scanStart, data.end(), vpattern.begin(), vpattern.end(),
                                       [&](uint8_t curr, std::pair<uint8_t, bool> currPattern)
                {
                        return (!currPattern.second) || curr == currPattern.first;
                });

                // Did we find a match..
                if (ret != data.end()) {
                        // If we hit the usage count, return the result..
                        if (result == count || count == 0) {
                                return (std::distance(data.begin(), ret) + base_address) + offset;
                        }

                        // Increment the found count and scan again..
                        ++result;
                        scanStart = ++ret;
                } else {
                        break;
                }
        }

        return 0;
}

/* Credits to user GuidedHacking: https://stackoverflow.com/questions/55803328/how-to-identify-what-parts-of-the-allocated-virtual-memory-a-process-is-using */
uintptr_t CHandleProcess::_FindExternalPattern(HANDLE h_proc, const char* pattern, uintptr_t begin, intptr_t size)
{
        uintptr_t match = 0;
        SIZE_T bytesRead;
        DWORD oldprotect;
        MEMORY_BASIC_INFORMATION mbi;
        mbi.RegionSize = 0x1000;

        VirtualQueryEx(h_proc, (LPCVOID)begin, &mbi, sizeof(mbi));

        for (uintptr_t curr = begin; curr < begin + size; curr += mbi.RegionSize) {
                if (!VirtualQueryEx(h_proc, (LPVOID)curr, &mbi, sizeof(mbi))) {
                        continue;
                }

                if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) {
                        continue;
                }

                std::vector<uint8_t> raw(mbi.RegionSize);

                if (VirtualProtectEx(h_proc, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect)) {
                        ReadProcessMemory(h_proc, mbi.BaseAddress, raw.data(), mbi.RegionSize, &bytesRead);
                        VirtualProtectEx(h_proc, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);

                        uintptr_t internalAddr = _FindPattern(std::ref(raw), 0, pattern, 0, 0);

                        if (internalAddr != 0) {
                                //calculate from internal to external
                                //match = curr + (internalAddr - reinterpret_cast<uintptr_t>(&raw[0]));
                                match = curr + internalAddr;
                                break;
                        }
                }
        }

        return match;
}