#pragma once
#include <cstddef>

namespace offsets {

	namespace offsets_hpp {
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1889F30;
		constexpr std::ptrdiff_t dwEntityList = 0x1A359C0;
		constexpr std::ptrdiff_t dwViewAngles = 0x1AABA50;
		constexpr std::ptrdiff_t dwViewMatrix = 0x1AA17C0;
	}

	namespace client_dll_hpp {
		constexpr std::ptrdiff_t m_iHealth = 0x344; // int32  
		constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8  
		constexpr std::ptrdiff_t m_hPawn = 0x62C;// CHandle<C_BasePlayerPawn> 
		constexpr std::ptrdiff_t m_hPlayerPawn = 0x80C; // CHandle<C_CSPlayerPawn>
		constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; // vector 
		constexpr std::ptrdiff_t m_iOldHealth = 0xAB4; // int32 
		constexpr std::ptrdiff_t m_iszPlayerName = 0x660; // char[128]
		constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0; // CNetworkViewOffsetVector
	}

	namespace engine2_dll {
		constexpr std::ptrdiff_t dwWindowHeight = 0x62354C;
		constexpr std::ptrdiff_t dwWindowWidth = 0x623548;
	}

}
