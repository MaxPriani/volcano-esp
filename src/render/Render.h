#pragma once
#include "../utils/Config.h"
#include "../utils/imguiDx.h"
#include "../utils/Resource.h"

class ESP;
class DLLAddresses;

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
    int resX;
    int resY;
    DXResources dxResources;
    Config& config;
    DLLAddresses& dllAddresses;
    explicit Render(Config& cfg, DLLAddresses& dll, int width, int height) : config(cfg), dllAddresses(dll), resX(width), resY(height) {}
public:
    void InitializeDirectX();
    void ReleaseResources();
    void Present();
    void RenderUI();
    void InitializeUI(HINSTANCE instance);
    void InitializeOverlayWindow(HINSTANCE instance);
    void UpdateOverlayResolution(int newX, int newY);
    HWND GetOverlayWindow() const;
    static LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};