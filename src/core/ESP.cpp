#include "ESP.h"

ESP::ESP(Memory& mem, Render* render, Config* config) :
    memory(mem),
    render(render),
    config(config),
    dllAddresses(mem),
    resX(0),
    resY(0) {
    resX = memory.Read<int>(dllAddresses.engine + offsets::engine2_dll::dwWindowWidth);
    resY = memory.Read<int>(dllAddresses.engine + offsets::engine2_dll::dwWindowHeight);
}

void ESP::UpdateGameState() {
    gameState.localPlayer = memory.Read<uintptr_t>(dllAddresses.client + offsets::client_dll::dwLocalPlayerPawn);
    gameState.viewMatrix = memory.Read<ViewMatrix>(dllAddresses.client + offsets::client_dll::dwViewMatrix);
    gameState.entityList = memory.Read<uintptr_t>(dllAddresses.client + offsets::client_dll::dwEntityList);
    gameState.localTeam = memory.Read<int>(gameState.localPlayer + offsets::client_dll::m_iTeamNum);
}

void ESP::DrawESPBox(const ImVec2& playerScreenPos, const ImVec2& playerScreenHead, ImU32 color) {

    float boxHeight = playerScreenPos.y - playerScreenHead.y;
    float boxWidth = boxHeight / 2.8f;

    ImGui::GetBackgroundDrawList()->AddRect(
        ImVec2(playerScreenHead.x - boxWidth / 2, playerScreenHead.y),
        ImVec2(playerScreenHead.x + boxWidth / 2, playerScreenPos.y),
        color,
        config->boxRoundness,
        0,
        config->boxThickness
    );

}

void ESP::DrawPlayerHealth(const ImVec2& playerScreenPos, int playerHealth, int playerTeam, float healthTextSize) {

    // Check if the player is an enemy and if showing enemy health is enabled
    bool firstCondition = (playerTeam != gameState.localTeam && config->showEnemyHealth);

    // Check if the player is a teammate and if showing teammate health and ESP for teammates are enabled
    bool secondCondition = (playerTeam == gameState.localTeam && config->showTeamHealth && config->showLocalTeam);

    if (firstCondition || secondCondition) {
        std::string healthText = std::to_string(playerHealth) + "HP";

        // Health color based on value
        if (playerHealth > 80) {
            config->healthColor = IM_COL32(0, 255, 0, 255);      // HP > 80 == Green
        }
        else if (playerHealth > 50) {
            config->healthColor = IM_COL32(255, 255, 0, 255);    // HP > 50 == Yellow
        }
        else if (playerHealth > 20) {
            config->healthColor = IM_COL32(255, 165, 0, 255);    // HP > 20 == Orange
        }
        else {
            config->healthColor = IM_COL32(255, 0, 0, 255);      // HP <= 20 == Red
        }

        // Draw health text
        ImGui::GetBackgroundDrawList()->AddText(
            ImGui::GetFont(),
            healthTextSize,
            ImVec2(playerScreenPos.x - (healthTextSize * healthText.length() / 4), playerScreenPos.y),
            config->healthColor,
            healthText.c_str()
        );
    }
}

void ESP::DrawPlayerDistance(const ImVec2& playerScreenPos, float playerDistance, float distanceTextSize) {

    if (config->enableDistance) {
        std::string enemyDistanceText = std::to_string(static_cast<int>(playerDistance));

        // Draw distance text
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

void ESP::DrawPlayerNames(const ImVec2& playerScreenPos, ImVec2 playerScreenHead, const char playerName[], float playerNamesTextSize) {

    if (config->showPlayerNames) {

        // Draw player names text
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

void ESP::HandleUIToggle() {

    if (GetAsyncKeyState(VK_F4) & 1) {
        uiActivated = !uiActivated;
        LONG windowStyle = GetWindowLong(render->GetOverlayWindow(), GWL_EXSTYLE);
        if (uiActivated) {
            windowStyle &= ~WS_EX_TRANSPARENT;  // Remove flag
        }
        else {
            windowStyle |= WS_EX_TRANSPARENT;   // Add flag
        }
        SetWindowLong(render->GetOverlayWindow(), GWL_EXSTYLE, windowStyle);
    }

}

void ESP::RenderESP() {

    HandleUIToggle();

    if (uiActivated) render->RenderUI();
    if (!config->enabledESP) return;

    UpdateGameState();

    for (int i = 0; i < 64; i++) {

        const auto listEntry = memory.Read<uintptr_t>(gameState.entityList + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!listEntry) continue;

        const auto entityController = memory.Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
        if (!entityController) continue;

        char playerName[128] = {};
        memory.ReadArray(entityController + offsets::client_dll::m_iszPlayerName, playerName, sizeof(playerName));

        const auto pawnHandle = memory.Read<uintptr_t>(entityController + offsets::client_dll::m_hPawn);
        if (!pawnHandle) continue;

        const auto entityPawn = memory.Read<uintptr_t>(listEntry + 120 * (pawnHandle & 0x1FF));
        if (!entityPawn) continue;
        if (entityPawn == gameState.localPlayer) continue;

        const auto playerTeam = memory.Read<int>(entityPawn + offsets::client_dll::m_iTeamNum);
        if (playerTeam == gameState.localTeam && !config->showLocalTeam) continue;

        const auto playerHealth = memory.Read<int>(entityPawn + offsets::client_dll::m_iHealth);
        if (playerHealth <= 0 || playerHealth > 100) continue;

        const Vector3 playerWorldPos = memory.Read<Vector3>(entityPawn + offsets::client_dll::m_vOldOrigin);

        Vector3 localPlayerWorldPos = memory.Read<Vector3>(gameState.localPlayer + offsets::client_dll::m_vOldOrigin);

        Vector3 playerWorldHead = playerWorldPos;
        playerWorldHead.z += 75.f;

        float playerDistance = localPlayerWorldPos.distance(playerWorldPos);
        if (config->enableDistance && playerDistance > config->maxEnemyDistance) continue;

        ImU32 color;
        if (playerTeam == gameState.localTeam) {
            color = ImGui::ColorConvertFloat4ToU32(config->teamColor);
        }
        else {
            color = ImGui::ColorConvertFloat4ToU32(config->enemyColor);
        }

        WorldToScreen wts(gameState.viewMatrix, resX, resY);
        ImVec2 playerScreenPos, playerScreenHead;

        // Transform both the player's world position and head into screen coordinates
        if ((wts.transform(playerWorldPos, playerScreenPos)) && (wts.transform(playerWorldHead, playerScreenHead))) {
            DrawESPBox(playerScreenPos, playerScreenHead, color);
            DrawPlayerHealth(playerScreenPos, playerHealth, playerTeam, config->healthTextSize);
            DrawPlayerDistance(playerScreenPos, playerDistance, config->distanceTextSize);
            DrawPlayerNames(playerScreenPos, playerScreenHead, playerName, config->playerNamesTextSize);
        }
    }
}