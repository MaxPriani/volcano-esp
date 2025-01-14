#pragma once
#include <cstddef>

namespace offsets {

	namespace client_dll {
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1869D88;
		constexpr std::ptrdiff_t dwEntityList = 0x1A157C8;
		constexpr std::ptrdiff_t dwViewAngles = 0x1A8A810;
		constexpr std::ptrdiff_t dwViewMatrix = 0x1A80870;
		constexpr std::ptrdiff_t m_iHealth = 0x344;
		constexpr std::ptrdiff_t m_iTeamNum = 0x3E3;
		constexpr std::ptrdiff_t m_hPawn = 0x62C;
		constexpr std::ptrdiff_t m_vOldOrigin = 0x1324;
		constexpr std::ptrdiff_t m_iOldHealth = 0xAB4; 
		constexpr std::ptrdiff_t m_iszPlayerName = 0x660; // char[128]
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowWidth = 0x616130;
		constexpr std::ptrdiff_t dwWindowHeight = 0x616134;
	}
}
