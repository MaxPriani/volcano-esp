#include "Memory.h"

using namespace winternl_h;

OBJECT_ATTRIBUTES Memory::InitObjectAttributes(PUNICODE_STRING name, ULONG attributes, HANDLE hRoot, PSECURITY_DESCRIPTOR security) {

    OBJECT_ATTRIBUTES object;

    object.Length = sizeof(OBJECT_ATTRIBUTES);
    object.ObjectName = name;
    object.Attributes = attributes;
    object.RootDirectory = hRoot;
    object.SecurityDescriptor = security;

    return object;

}

DWORD Memory::GetPID(const wchar_t* processName) {

    HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, false);

    if (handleSnap != INVALID_HANDLE_VALUE) {

        PROCESSENTRY32 processEntry = {};
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        while (Process32Next(handleSnap, &processEntry)) {

            if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(handleSnap);
                return processEntry.th32ProcessID;
            }

        }
    }

    CloseHandle(handleSnap);
    return 0;

}

const std::uintptr_t Memory::GetModuleAddress(const wchar_t* moduleName) const noexcept {

    HANDLE handleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, targetpID);

    if (handleSnap != INVALID_HANDLE_VALUE) {

        ::MODULEENTRY32 moduleEntry = {};
        moduleEntry.dwSize = sizeof(::MODULEENTRY32);

        std::uintptr_t result = 0;

        while (::Module32Next(handleSnap, &moduleEntry)) {
            if (_wcsicmp(moduleName, moduleEntry.szModule) == 0) {
                result = reinterpret_cast<std::uintptr_t>(moduleEntry.modBaseAddr);
                ::CloseHandle(handleSnap);
                return result;
            }
        }

    }

    ::CloseHandle(handleSnap);
    return 0;
}

bool Memory::IsHandleValid(HANDLE handle) {
    return handle && handle != INVALID_HANDLE_VALUE;
}

HANDLE Memory::GetHijackedHandle(DWORD targetPID) {

    HANDLE hDuplicated = nullptr;
    HANDLE hOwner = nullptr;

    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

    auto RtlAdjustPrivilege = reinterpret_cast<_RtlAdjustPrivilege>(GetProcAddress(ntdll, "RtlAdjustPrivilege"));
    auto NtQuerySystemInformation = reinterpret_cast<_NtQuerySystemInformation>(GetProcAddress(ntdll, "NtQuerySystemInformation"));
    auto NtDuplicateObject = reinterpret_cast<_NtDuplicateObject>(GetProcAddress(ntdll, "NtDuplicateObject"));
    auto NtOpenProcess = reinterpret_cast<_NtOpenProcess>(GetProcAddress(ntdll, "NtOpenProcess"));

    BOOLEAN oldPrivilege;
    RtlAdjustPrivilege(SeDebugPriv, TRUE, FALSE, &oldPrivilege);

    DWORD size = sizeof(SYSTEM_HANDLE_INFORMATION);

    PSYSTEM_HANDLE_INFORMATION allHandlesInfo= nullptr;
    NTSTATUS status = NULL;
    while (true) {

        if (allHandlesInfo) {
            delete[] reinterpret_cast<unsigned char*>(allHandlesInfo);
        }

        allHandlesInfo = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(new unsigned char[size]);

        status = NtQuerySystemInformation(SystemHandleInformation, allHandlesInfo, size, nullptr);

        if (status == STATUS_SUCCESS) {
            break;
        }

        size *= 1.5;
    }

    auto steamPID = GetPID(L"steam.exe");
    for (ULONG i = 0; i < allHandlesInfo->HandleCount; ++i) {

        const auto& handle = allHandlesInfo->Handles[i];

        if (!IsHandleValid(reinterpret_cast<HANDLE>(handle.Handle))) {
            continue;
        }

        if (handle.ProcessId != steamPID) {
            continue;
        }

        if (handle.ObjectTypeNumber != ProcessHandleType) {
            continue;
        }

        if ((handle.GrantedAccess & (PROCESS_VM_READ | PROCESS_QUERY_INFORMATION)) == 0) {
            continue;
        }

        OBJECT_ATTRIBUTES objAttributes = InitObjectAttributes(NULL, NULL, NULL, NULL);
        CLIENT_ID clientId = { };
        clientId.UniqueProcess = (reinterpret_cast<DWORD*>(handle.ProcessId));

        status = NtOpenProcess(&hOwner, PROCESS_DUP_HANDLE, &objAttributes, &clientId);

        if (!IsHandleValid(hOwner) || !NT_SUCCESS(status)) {
            continue;
        }

        status = NtDuplicateObject(
            hOwner,
            reinterpret_cast<HANDLE>(handle.Handle),
            NtCurrentProcess,
            &hDuplicated,
            PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
            0,
            0);

        if (!IsHandleValid(hDuplicated) || !NT_SUCCESS(status)) {
            CloseHandle(hOwner);
            continue;
        }

        CloseHandle(hOwner);

        if (GetProcessId(hDuplicated) == targetPID) {
            delete[] allHandlesInfo;
            return hDuplicated;
        }

        CloseHandle(hDuplicated);
        continue;
    }

    return nullptr;
}

Memory::Memory(const wchar_t* processName) noexcept {
    targetpID = GetPID(processName);
    if (targetpID != 0) {
        hHijacked = GetHijackedHandle(targetpID);
    }
}

Memory::~Memory() {
    if (hHijacked) CloseHandle(hHijacked);
}