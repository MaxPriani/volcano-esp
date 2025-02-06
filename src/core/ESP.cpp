#include "ESP.h"

ESP::ESP(Memory& mem, Render* render, Config* config) :
    memory(mem),
    render(render),
    config(config),
    gameState(mem), 
    resX(GetResX()),
    resY(GetResY()) {}

int ESP::GetResX() {
    return memory.Read<int>(gameState.GetEngineDLL() + offsets::engine2_dll::dwWindowWidth);
}

int ESP::GetResY() {
    return memory.Read<int>(gameState.GetEngineDLL() + offsets::engine2_dll::dwWindowHeight);
}

Player::Player(uintptr_t entityPawn, int team, int health,
    Vector3 worldPos, Vector3 worldHead, float distance,
    std::string name)
    : entityPawn(entityPawn), team(team), health(health),
    worldPos(worldPos), worldHead(worldHead), distance(distance), name(name) {
}

Player ESP::GetPlayerInfo(int index) {

    const auto listEntry = memory.Read<uintptr_t>(gameState.entityList + (8 * (index & 0x7FFF) >> 9) + 16);
    if (!listEntry) return Player();

    const auto entityController = memory.Read<uintptr_t>(listEntry + 120 * (index & 0x1FF));
    if (!entityController) return Player();

    char playerName[128] = {};
    memory.ReadArray(entityController + offsets::client_dll_hpp::m_iszPlayerName, playerName, sizeof(playerName));

    const auto pawnHandle = memory.Read<uintptr_t>(entityController + offsets::client_dll_hpp::m_hPawn);
    if (!pawnHandle) return Player();

    const auto entityPawn = memory.Read<uintptr_t>(listEntry + 120 * (pawnHandle & 0x1FF));
    if (!entityPawn) return Player();

    int team = memory.Read<int>(entityPawn + offsets::client_dll_hpp::m_iTeamNum);
    int health = memory.Read<int>(entityPawn + offsets::client_dll_hpp::m_iHealth);
    Vector3 worldPos = memory.Read<Vector3>(entityPawn + offsets::client_dll_hpp::m_vOldOrigin);

    Vector3 worldHead = worldPos;
    worldHead.z += 77.f;

    Vector3 localPlayerWorldPos = memory.Read<Vector3>(gameState.localPlayer + offsets::client_dll_hpp::m_vOldOrigin);
    float distance = localPlayerWorldPos.distance(worldPos);

    return Player(entityPawn, team, health, worldPos, worldHead, distance, std::string(playerName));
}

bool Player::IsEmpty() {
    return entityPawn == 0;
}

bool ESP::ShouldSkipPlayer(Player& player) {
    if (player.entityPawn == gameState.localPlayer) return true;
    if (player.health <= 0 || player.health > 100) return true;
    if (player.team == gameState.localTeam && !config->showLocalTeam) return true;
    if (config->enableDistance && player.distance > config->maxEnemyDistance) return true;
    return false;
}

void ESP::ProcessPlayer(Player& player) {

    if (player.IsEmpty()) return;
    if (ShouldSkipPlayer(player)) return;
    RenderPlayerESP(player);

}

void ESP::RenderESP() {

    HandleUIToggle();

    if (uiActivated) render->RenderUI();
    if (!config->enabledESP) return;

    gameState.Update();

    for (int i = 0; i < 64; i++) {
        Player player = GetPlayerInfo(i);
        ProcessPlayer(player);
    }

}

ImU32 HPBasedColor(int health) {

    if (health > 80) {
        return IM_COL32(0, 255, 0, 255);
    }
    else if (health > 50) {
        return IM_COL32(255, 255, 0, 255);
    }
    else if (health > 20) {
        return IM_COL32(255, 165, 0, 255);
    }
    else {
        return IM_COL32(255, 0, 0, 255);
    }

}

ImU32 ESP::PlayerColor(Player& player) {

    ImU32 color;
    if (config->enableHealthBased) {
        color = HPBasedColor(player.health);
    }
    else if (player.team == gameState.localTeam) {
        color = ImGui::ColorConvertFloat4ToU32(config->teamColor);
    }
    else {
        color = ImGui::ColorConvertFloat4ToU32(config->enemyColor);
    }

    return color;
}

void ESP::DrawHealthText(const ImVec2& playerScreenPos, int playerHealth, int playerTeam, float healthTextSize) {

    if (config->showHealth) {

        std::string healthText = std::to_string(playerHealth) + "HP";
        config->healthColor = HPBasedColor(playerHealth);

        ImGui::GetBackgroundDrawList()->AddText(
            ImGui::GetFont(),
            config->healthTextSize,
            ImVec2(playerScreenPos.x - (healthTextSize * healthText.length() / 4), playerScreenPos.y),
            config->healthColor,
            healthText.c_str()
        );

    }

}

void ESP::DrawESPBox(const ImVec2& playerScreenPos, const ImVec2& playerScreenHead, ImU32 color) {

    float boxHeight = playerScreenPos.y - playerScreenHead.y;
    float boxWidth = boxHeight / 2.4f;

    ImGui::GetBackgroundDrawList()->AddRect(
        ImVec2(playerScreenHead.x - boxWidth / 2, playerScreenHead.y),
        ImVec2(playerScreenHead.x + boxWidth / 2, playerScreenPos.y),
        color,
        config->boxRoundness,
        0,
        config->boxThickness
    );

}

void ESP::DrawPlayerDistance(const ImVec2& playerScreenPos, float playerDistance, float distanceTextSize) {

    if (config->enableDistance) {
        std::string enemyDistanceText = std::to_string(static_cast<int>(playerDistance));

        ImGui::GetBackgroundDrawList()->AddText(
            ImGui::GetFont(),
            config->distanceTextSize,
            ImVec2(playerScreenPos.x - (config->distanceTextSize * enemyDistanceText.length() / 4),
                playerScreenPos.y + config->distanceTextSize),
            config->distanceTextColor,
            enemyDistanceText.c_str()
        );

    }
}

void ESP::DrawPlayerNames(const ImVec2& playerScreenPos, ImVec2 playerScreenHead, int playerHealth, const char playerName[], float playerNamesTextSize) {

    if (config->showPlayerNames) {

        if (config->enableHealthBased) {
            config->playerNamesTextColor = HPBasedColor(playerHealth);
        }

        ImGui::GetBackgroundDrawList()->AddText(
            ImGui::GetFont(),
            playerNamesTextSize,
            ImVec2(playerScreenHead.x - (playerNamesTextSize * strlen(playerName) / 4),
                playerScreenHead.y - playerNamesTextSize),
            config->playerNamesTextColor,
            playerName

        );

    }
}

void ESP::RenderPlayerESP(Player& player) {

    ImU32 color = PlayerColor(player);

    WorldToScreen wts(gameState.viewMatrix, resX, resY);
    ImVec2 playerScreenPos, playerScreenHead;

    if (wts.transform(player.worldPos, playerScreenPos) && wts.transform(player.worldHead, playerScreenHead)) {
        DrawESPBox(playerScreenPos, playerScreenHead, color);
        DrawHealthText(playerScreenPos, player.health, player.team, config->healthTextSize);
        DrawPlayerDistance(playerScreenPos, player.distance, config->distanceTextSize);
        DrawPlayerNames(playerScreenPos, playerScreenHead, player.health, player.name.c_str(), config->playerNamesTextSize);
    }

}

void ESP::HandleUIToggle() {

    static bool previousState = false; 

    bool currentState = (GetAsyncKeyState(VK_XBUTTON1) & 1);

    
    if (currentState && !previousState) {
        config->enabledESP = !config->enabledESP; 
    }

    previousState = currentState;

    if (GetAsyncKeyState(VK_F1) & 1 || (GetAsyncKeyState(VK_XBUTTON2) & 1)) {
        uiActivated = !uiActivated;
        LONG windowStyle = GetWindowLong(render->GetOverlayWindow(), GWL_EXSTYLE);
        if (uiActivated) {
            windowStyle &= ~WS_EX_TRANSPARENT;  
        }
        else {
            windowStyle |= WS_EX_TRANSPARENT;  
        }
        SetWindowLong(render->GetOverlayWindow(), GWL_EXSTYLE, windowStyle);
    }

}