#pragma once
#include <string>
#include <imgui.h>
#include "Game.h"
#include "../utils/imguiDx.h"
#include "../utils/Config.h"
#include "../math/Vec3.h"
#include "../memory/Memory.h"
#include "../memory/Offsets.h"
#include "../render/Render.h"

class Player {
public:
    uintptr_t entityPawn;
    int team;
    int health;
    Vector3 worldPos;
    Vector3 worldHead;
    float distance;
    std::string name;
public:
    Player(uintptr_t entityPawn, int team, int health, Vector3 worldPos, Vector3 worldHead, float distance, std::string name);
    Player() = default; 
    bool IsEmpty();
};

class ESP {
private:
    Memory& memory;
    Config* config;
    GameState gameState;
    Player player;
    bool uiActivated = true;

    void HandleUIToggle();
    void ProcessPlayer(Player& player);
    void RenderPlayerESP(Player& player);
    bool ShouldSkipPlayer(Player& player);
    Player GetPlayerInfo(int index);
    ImU32 PlayerColor(Player& player);

public:
    int resX;
    int resY;
    Render* render;
    ESP(Memory& mem, Render* render, Config* config);
    int GetResX();
    int GetResY();
    void RenderESP();
    void DrawESPBox(const ImVec2& playerScreenPos, const ImVec2& playerScreenHead, ImU32 color);
    void DrawHealthText(const ImVec2& playerScreenPos, int health, int team, float healthTextSize);
    void DrawPlayerDistance(const ImVec2& playerScreenPos, float playerDistance, float distanceTextSize);
    void DrawPlayerNames(const ImVec2& playerScreenPos, ImVec2 playerScreenHead, int playerHealth, const char playerName[], float playerNamesTextSize);
};