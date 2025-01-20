#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

class Memory {

public:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;

public:

	Memory(const wchar_t* processName) noexcept;
	~Memory();
	const std::uintptr_t GetModuleAddress(const wchar_t* moduleName) const noexcept;

	// Read process memory
	template <typename T>
	const T Read(const std::uintptr_t address) const noexcept {

		T value = { };

		if (processHandle) {
			::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		}
		return value;
	}

	// Read array of bytes
	template<typename T>
	bool ReadArray(const std::uintptr_t address, T array[], const size_t n) const noexcept {
		if (processHandle) {
			return ::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), array, n * sizeof(T), NULL);
		}
		return false;
	}
	
};

