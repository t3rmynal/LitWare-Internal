#pragma once
#include <string>
#include <functional>
#include <d3d11.h>
#include "../include/cefsimple.h"

class CEFManager {
public:
    static CEFManager& Get();

    bool Initialize(const std::string& resourcePath);
    void Shutdown();
    bool IsReady() const { return m_initialized; }

    // Render
    void Update();
    void Render(ID3D11RenderTargetView* rtv, ID3D11DeviceContext* ctx, uint32_t width, uint32_t height);

    // Message passing
    void SendMessage(const std::string& jsonMessage);
    void SetMessageCallback(std::function<void(const std::string&)> callback);

    // Input
    void OnMouseMove(int x, int y);
    void OnMouseClick(int x, int y, bool down, int button);
    void OnMouseWheel(int x, int y, int delta);
    void OnKeyEvent(int keyCode, int modifiers, bool down);

    // Texture access
    ID3D11Texture2D* GetTexture() const { return m_surfaceTexture; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    CEFManager();
    ~CEFManager();

    bool m_initialized = false;
    int m_width = 640;
    int m_height = 480;

    // CEF handles
    CefRefPtr<CefBrowser> m_browser;
    CefRefPtr<CefApp> m_cefApp;

    // D3D resources
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    ID3D11Texture2D* m_surfaceTexture = nullptr;
    ID3D11ShaderResourceView* m_surfaceSRV = nullptr;

    std::function<void(const std::string&)> m_messageCallback;
};
