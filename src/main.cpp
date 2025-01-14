#include "core/ESP.h"
#include "render/Render.h"
#include "utils/Config.h"
#include "memory/Memory.h"

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    Memory memory(L"cs2.exe");

    Config config;
    DLLAddresses dllAddresses(memory);

    ESP esp(memory, nullptr, &config);

    Render render(config, dllAddresses, esp.resX, esp.resY);
    esp.render = &render;

    render.InitializeUI(instance);  

    ShowWindow(render.GetOverlayWindow(), cmd_show);
    UpdateWindow(render.GetOverlayWindow());

    bool running = true;
    while (running) {
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
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return 0;
}