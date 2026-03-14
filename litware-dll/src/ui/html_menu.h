#pragma once
#include <string>
#include <functional>
#include <memory>
#include <d3d11.h>
#include <imgui.h>

// HTML Menu wrapper - manages CEF browser and rendering
class HTMLMenu {
public:
    HTMLMenu();
    ~HTMLMenu();

    // Initialize CEF and load HTML
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& htmlPath);
    void Shutdown();

    // Update and render
    void Update();
    void Render(ImDrawList* drawList, ImVec2 pos, ImVec2 size);

    // Input handling
    void OnMouseMove(int x, int y);
    void OnMouseClick(int x, int y, bool down, int button);
    void OnMouseWheel(int delta);
    void OnKeyDown(int vkCode);
    void OnKeyUp(int vkCode);

    // Message communication (C++ <-> JavaScript)
    void SendMessage(const std::string& jsonMessage);
    void SetMessageCallback(std::function<void(const std::string&)> callback);

    // State
    bool IsInitialized() const { return m_initialized; }
    bool IsLoaded() const { return m_loaded; }

private:
    bool m_initialized = false;
    bool m_loaded = false;

    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;

    // Render texture
    ID3D11Texture2D* m_renderTexture = nullptr;
    ID3D11RenderTargetView* m_renderRTV = nullptr;
    ID3D11ShaderResourceView* m_renderSRV = nullptr;

    int m_width = 640;
    int m_height = 480;

    std::function<void(const std::string&)> m_messageCallback;

    // CEF browser instance (when CEF is integrated)
    void* m_browserHandle = nullptr;

    ImTextureID m_imguiTextureId = nullptr;
};
