#include "Game.h"

uintptr_t GameState::GetClientDLL() {
    return memory.GetModuleAddress(L"client.dll");
}

uintptr_t GameState::GetEngineDLL() {
    return memory.GetModuleAddress(L"engine2.dll");
}

ViewMatrix GameState::GetViewMatrix() {
    return memory.Read<ViewMatrix>(GetClientDLL() + offsets::offsets_hpp::dwViewMatrix);
}

uintptr_t GameState::GetLocalPlayer() {
    return memory.Read<uintptr_t>(GetClientDLL() + offsets::offsets_hpp::dwLocalPlayerPawn);
}

uintptr_t GameState::GetEntityList() {
    return memory.Read<uintptr_t>(GetClientDLL() + offsets::offsets_hpp::dwEntityList);
}

int GameState::GetLocalTeam() {
    return memory.Read<int>(localPlayer + offsets::client_dll_hpp::m_iTeamNum);
}

void GameState::Update() {
    viewMatrix = GetViewMatrix();
    localPlayer = GetLocalPlayer();
    entityList = GetEntityList();
    localTeam = GetLocalTeam();
}