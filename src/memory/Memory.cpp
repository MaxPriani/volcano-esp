#include "Memory.h"

// Constructor that finds the PID and opens a handle
Memory::Memory(const wchar_t* processName) noexcept {

    ::PROCESSENTRY32 entry = {};
    entry.dwSize = sizeof(::PROCESSENTRY32);

    const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Iterate through all processes in the system snapshot
    bool processFound = false;
    while (::Process32Next(snapShot, &entry)) {

        // If process is found, store its ID and get handle with full access
        if (_wcsicmp(processName, entry.szExeFile) == 0) {
            processId = entry.th32ProcessID;
            processFound = true;

            // OpenProcess gives us access to the process memory, threads and other resources
            processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
            break;
        }
    }

    // Free handle
    if (snapShot)
        ::CloseHandle(snapShot);
}

// Destructor that frees the opened handle
Memory::~Memory() {
    if (processHandle)
        ::CloseHandle(processHandle);
}

// Returns the base address of a specified module (DLL), or 0 if not found
const std::uintptr_t Memory::GetModuleAddress(const wchar_t* moduleName) const noexcept {

    ::MODULEENTRY32 entry = {};
    entry.dwSize = sizeof(::MODULEENTRY32);

    const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

    std::uintptr_t result = 0;
    while (::Module32Next(snapShot, &entry)) {

        // Compare module names and store base address if matched
        if (_wcsicmp(moduleName, entry.szModule) == 0) {
            result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
            break;
        }

    }

    if (snapShot)
        ::CloseHandle(snapShot);

    return result;
}