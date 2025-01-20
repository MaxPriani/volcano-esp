#pragma once
#include <string>
#include <imgui.h>
#include "../utils/imguiDx.h"
#include "../utils/Config.h"
#include "../math/Vec3.h"
#include "../memory/Memory.h"
#include "../memory/Offsets.h"
#include "../render/Render.h"

struct GameState {
    uintptr_t localPlayer;
    ViewMatrix viewMatrix;
    uintptr_t entityList;
    int localTeam;
};

class DLLAddresses {
public:
    const uintptr_t client;
    const uintptr_t engine;
public:
    DLLAddresses(Memory& memory)
        : client(memory.GetModuleAddress(L"client.dll")), engine(memory.GetModuleAddress(L"engine2.dll")) {}
};

class ESP {
public:
    Memory& memory;
    Render* render;
    Config* config;
    DLLAddresses dllAddresses;
    int resX;
    int resY;
    GameState gameState;
    Player player;
    bool uiActivated = true;
    ESP(Memory& mem, Render* render, Config* config);
public:
    void RenderESP();
    void DrawESPBox(const ImVec2& playerScreenPos, const ImVec2& playerScreenHead, ImU32 color);
    void UpdateGameState();
    void HandleUIToggle();
    void DrawPlayerHealth(const ImVec2& playerScreenPos, int health, int team, float healthTextSize);
    void DrawPlayerDistance(const ImVec2& playerScreenPos, float playerDistance, float distanceTextSize);
    void DrawPlayerNames(const ImVec2& playerScreenPos, ImVec2 playerScreenHead, const char playerName[], float playerNamesTextSize);
};