#include "Render.h"
#include "../core/ESP.h"

Render::Render(Config& cfg) :
    config(cfg) {}

void Render::InitializeDirectX(ESP& esp) {

    DXGI_SWAP_CHAIN_DESC swapChainDescription = {
    .BufferDesc = {
        .Width = static_cast<UINT>(esp.resX),
        .Height = static_cast<UINT>(esp.resY),
        .RefreshRate = {144, 1},
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM
    },
    .SampleDesc = {1, 0},
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = 2,
    .OutputWindow = dxResources.window,
    .Windowed = TRUE,
    .SwapEffect = DXGI_SWAP_EFFECT_DISCARD
    };

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL featureLevel;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 2, D3D11_SDK_VERSION,
        &swapChainDescription, &dxResources.swapChain, &dxResources.device, &featureLevel, &dxResources.context);

    ID3D11Texture2D* backBuffer = nullptr;
    dxResources.swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

    if (backBuffer) {
        dxResources.device->CreateRenderTargetView(backBuffer, nullptr, &dxResources.renderTargetView);
        backBuffer->Release();
    }

}

void Render::ReleaseResources() {
    if (dxResources.device) dxResources.device->Release();
    if (dxResources.context) dxResources.context->Release();
    if (dxResources.swapChain) dxResources.swapChain->Release();
    if (dxResources.renderTargetView) dxResources.renderTargetView->Release();
}

LRESULT CALLBACK Render::HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam)) return 0L;

    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0L;
    }
    return DefWindowProc(window, message, wParam, lParam);
}

void Render::InitializeOverlayWindow(HINSTANCE instance, ESP& esp) {

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = HandleMessage;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON1));
    wc.lpszClassName = L"ESP";

    RegisterClassExW(&wc);

    dxResources.window = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        wc.lpszClassName,
        L"ESP",
        WS_POPUP,
        0, 0, esp.resX, esp.resY,
        nullptr, nullptr,
        instance, nullptr
    );


    SetLayeredWindowAttributes(dxResources.window, RGB(0, 0, 0), 255, LWA_COLORKEY);
}

void Render::Present() {
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    dxResources.context->OMSetRenderTargets(1, &dxResources.renderTargetView, nullptr);
    dxResources.context->ClearRenderTargetView(dxResources.renderTargetView, clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    dxResources.swapChain->Present(1, 0);
}

HWND Render::GetOverlayWindow() const {
    return dxResources.window;
}

void Render::InitializeUI(HINSTANCE instance, ESP& esp) {

    InitializeOverlayWindow(instance, esp);
    InitializeDirectX(esp);
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(dxResources.window);
    ImGui_ImplDX11_Init(dxResources.device, dxResources.context);

}

void Render::RenderUI() {       

    ImGui::SetNextWindowSize(ImVec2(380, 220), ImGuiCond_Always);
    if (ImGui::Begin("Settings |         Volcano's ESP         | by flosur", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::BeginTabBar("##ESPTabs")) {
            MainTab();
            ESPRangeTab();
            HUDTab();
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void Render::MainTab() {

    if (ImGui::BeginTabItem("ESP")) {
        ImGui::Checkbox("Enable ESP", &config.enabledESP);
        ImGui::Checkbox("Enable HP-based Color", &config.enableHealthBased);
        ImGui::SliderFloat("Box Thickness", &config.boxThickness, 1.0f, 5.0f, "%.1f");
        ImGui::SliderFloat("Box Roudness", &config.boxRoundness, 0.0f, 100.0f, "%.1f");
        ImGui::ColorEdit4("Team Box", reinterpret_cast<float*>(&config.teamColor));
        ImGui::ColorEdit4("Enemy Box", reinterpret_cast<float*>(&config.enemyColor));
        ImGui::EndTabItem();
    }
}

void Render::ESPRangeTab() {

    if (ImGui::BeginTabItem("Range")) {
        ImGui::Checkbox("Enable Distance Limit", &config.enableDistance);
        ImGui::SliderFloat("Max Enemy Distance", &config.maxEnemyDistance, 300.0f, 6000.0f, "%.1f");
        ImGui::SliderFloat("Distance Text Size", &config.distanceTextSize, 15.0f, 25.0f, "%.1f");
        ImGui::ColorEdit4("Distance Text Color", reinterpret_cast<float*>(&config.distanceTextColor));
        ImGui::EndTabItem();
    }
}

void Render::HUDTab() {

    if (ImGui::BeginTabItem("HUD")) {
        ImGui::Checkbox("Show Teammates", &config.showLocalTeam);
        ImGui::Checkbox("Show Player Names", &config.showPlayerNames);
        ImGui::Checkbox("Show Health", &config.showHealth);
        ImGui::SliderFloat("Health Text Size", &config.healthTextSize, 15.0f, 25.0f, "%.1f");
        ImGui::SliderFloat("Player Names Text Size", &config.playerNamesTextSize, 15.0f, 25.0f, "%.1f");
        ImGui::ColorEdit4("Player Names Color", reinterpret_cast<float*>(&config.playerNamesTextColor));
        ImGui::EndTabItem();
    }

}


