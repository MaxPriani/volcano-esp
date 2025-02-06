#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <sstream>
#include "Offsets.h"

#define SeDebugPriv 20
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004)
#define NtCurrentProcess ( (HANDLE)(LONG_PTR)-1 )
#define ProcessHandleType 0x8
#define SystemHandleInformation 16
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

namespace winternl_h {

    typedef struct _UNICODE_STRING {

        USHORT Length;
        USHORT MaximumLength;
        PWCH Buffer;

    } UNICODE_STRING, * PUNICODE_STRING;

    typedef struct _OBJECT_ATTRIBUTES {

        ULONG Length;
        HANDLE RootDirectory;
        PUNICODE_STRING ObjectName;
        ULONG Attributes;
        PVOID SecurityDescriptor;
        PVOID SecurityQualityOfService;

    } OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

    typedef struct _CLIENT_ID {

        PVOID UniqueProcess;
        PVOID UniqueThread;

    } CLIENT_ID, * PCLIENT_ID;

    typedef struct  _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
        ULONG ProcessId;
        BYTE ObjectTypeNumber;
        BYTE Flags;
        USHORT Handle;
        PVOID Object;
        ACCESS_MASK GrantedAccess;
    } _SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE;

    typedef struct _SYSTEM_HANDLE_INFORMATION {

        ULONG HandleCount;
        _SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];

    } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;


    typedef NTSTATUS(NTAPI* _RtlAdjustPrivilege)(

        ULONG Privilege,
        BOOLEAN Enable,
        BOOLEAN CurrentThread,
        PBOOLEAN Enabled

        );


    typedef NTSTATUS(NTAPI* _NtDuplicateObject)(

        HANDLE SourceProcessHandle,
        HANDLE SourceHandle,
        HANDLE TargetProcessHandle,
        PHANDLE TargetHandle,
        ACCESS_MASK DesiredAccess,
        ULONG Attributes,
        ULONG Options

        );

    typedef NTSYSAPI NTSTATUS(NTAPI* _NtOpenProcess)(

        PHANDLE ProcessHandle,
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        PCLIENT_ID ClientId

        );

    typedef NTSTATUS(NTAPI* _NtQuerySystemInformation)(

        ULONG SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength

        );

}

class Memory {
private:
    std::uintptr_t targetpID = 0;
    HANDLE hHijacked = nullptr;

    winternl_h::OBJECT_ATTRIBUTES InitObjectAttributes(winternl_h::PUNICODE_STRING name, ULONG attributes, HANDLE hRoot, PSECURITY_DESCRIPTOR security);
    bool IsHandleValid(HANDLE handle);
    HANDLE GetHijackedHandle(DWORD targetPID);

public:
    Memory(const wchar_t* processName) noexcept;
    ~Memory();
    DWORD GetPID(const wchar_t* procName);
    const std::uintptr_t GetModuleAddress(const wchar_t* moduleName) const noexcept;

    template <typename T>
    const T Read(const std::uintptr_t address) const noexcept {

        T value = { };

        if (hHijacked) {
            ::ReadProcessMemory(hHijacked, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), NULL);
        }

        return value;
    }

    template<typename T>
    bool ReadArray(const std::uintptr_t address, T array[], const size_t n) noexcept {

        if (hHijacked) {
            return ::ReadProcessMemory(hHijacked, reinterpret_cast<LPCVOID>(address), array, n * sizeof(T), NULL);
        }

        return false;
    }

};