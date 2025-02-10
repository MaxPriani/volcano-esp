#pragma once
#include "../memory/Memory.h"
#include "../memory/Offsets.h"
#include "../math/Vec3.h"

class GameState {
public:
	Memory& memory;
	ViewMatrix viewMatrix;
	uintptr_t localPlayer;
	uintptr_t entityList;
	int localTeam;

public:
	GameState(Memory& mem) : memory(mem) {}
	uintptr_t GetClientDLL();
	uintptr_t GetEngineDLL();
	ViewMatrix GetViewMatrix();
	uintptr_t GetLocalPlayer();
	uintptr_t GetEntityList();
	int GetLocalTeam();
	void Update();
};

