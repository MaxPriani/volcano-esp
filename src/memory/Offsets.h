#pragma once
#include <cstddef>

namespace offsets {

	namespace client_dll {
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x186BDF8;
		constexpr std::ptrdiff_t dwEntityList = 0x1A176C8;
		constexpr std::ptrdiff_t dwViewAngles = 0x1A82740;
		constexpr std::ptrdiff_t dwViewMatrix = 0x1A82740;
		constexpr std::ptrdiff_t m_iHealth = 0x344; // int32  
		constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8  
		constexpr std::ptrdiff_t m_hPawn = 0x62C;// CHandle<C_BasePlayerPawn> 
		constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; // vector 
		constexpr std::ptrdiff_t m_iOldHealth = 0xAB4; // int32 
		constexpr std::ptrdiff_t m_iszPlayerName = 0x660; // char[128]
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowWidth = 0x61D398;
		constexpr std::ptrdiff_t dwWindowHeight = 0x61D39C;
	}
}