#pragma once
#include "../utils/Config.h"
#include "../utils/imguiDx.h"
#include "../utils/Resource.h"

class ESP;

class DXResources {
public:
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    HWND window = nullptr;
};

class Render {
public:
    Config& config;
    DXResources dxResources;
public:
    Render(Config& cfg);
    void InitializeDirectX(ESP& esp);
    void ReleaseResources();
    void Present();
    void RenderUI();
    void MainTab();
    void ESPRangeTab();
    void HUDTab();
    void InitializeUI(HINSTANCE instance, ESP& esp);
    void InitializeOverlayWindow(HINSTANCE instance, ESP& esp);
    HWND GetOverlayWindow() const;
    static LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};