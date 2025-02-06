#include <chrono>
#include <thread>
#include "core/ESP.h"
#include "render/Render.h"
#include "utils/Config.h"
#include "memory/Memory.h"

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {

    Memory memory(L"cs2.exe");
    Config config;
    GameState gameState(memory);
    Render render(config);
    ESP esp(memory, &render, &config);

    render.InitializeUI(instance, esp);

    ShowWindow(render.GetOverlayWindow(), cmd_show);
    UpdateWindow(render.GetOverlayWindow());

    bool running = true;
    int FPS = 120;  
    std::chrono::milliseconds frameDuration(1000 / FPS);
    while (running) {

        auto frameStart = std::chrono::high_resolution_clock::now();

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        esp.RenderESP();
        ImGui::Render();
        render.Present();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> elapsedTime = frameEnd - frameStart;

        if (elapsedTime < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsedTime);
        }

    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}