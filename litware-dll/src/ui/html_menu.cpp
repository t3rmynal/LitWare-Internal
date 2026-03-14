#include "html_menu.h"
#include <d3d11.h>
#include <fstream>
#include <sstream>

HTMLMenu::HTMLMenu() = default;

HTMLMenu::~HTMLMenu() {
    Shutdown();
}

bool HTMLMenu::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& htmlPath) {
    if (m_initialized) return true;

    m_device = device;
    m_context = context;

    // Create render texture for HTML output
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = m_width;
    texDesc.Height = m_height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;

    if (FAILED(device->CreateTexture2D(&texDesc, nullptr, &m_renderTexture))) {
        return false;
    }

    // Create render target view
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(device->CreateRenderTargetView(m_renderTexture, &rtvDesc, &m_renderRTV))) {
        m_renderTexture->Release();
        return false;
    }

    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    if (FAILED(device->CreateShaderResourceView(m_renderTexture, &srvDesc, &m_renderSRV))) {
        m_renderRTV->Release();
        m_renderTexture->Release();
        return false;
    }

    // Register texture with ImGui
    m_imguiTextureId = (ImTextureID)m_renderSRV;

    // Load and parse HTML file
    std::ifstream file(htmlPath);
    if (!file.is_open()) {
        m_renderSRV->Release();
        m_renderRTV->Release();
        m_renderTexture->Release();
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string htmlContent = buffer.str();

    // TODO: Parse and process HTML content
    // For now, we'll initialize with placeholder rendering

    m_initialized = true;
    m_loaded = true;

    return true;
}

void HTMLMenu::Shutdown() {
    if (m_renderSRV) {
        m_renderSRV->Release();
        m_renderSRV = nullptr;
    }
    if (m_renderRTV) {
        m_renderRTV->Release();
        m_renderRTV = nullptr;
    }
    if (m_renderTexture) {
        m_renderTexture->Release();
        m_renderTexture = nullptr;
    }
    m_initialized = false;
}

void HTMLMenu::Update() {
    if (!m_initialized) return;

    // Clear render target with dark background
    const float clearColor[] = { 0.05f, 0.07f, 0.1f, 1.0f };
    m_context->ClearRenderTargetView(m_renderRTV, clearColor);

    // TODO: Render HTML content to texture
    // This would involve:
    // 1. Parsing HTML elements
    // 2. Rendering them using Direct2D or custom D3D
    // 3. Updating the texture
}

void HTMLMenu::Render(ImDrawList* drawList, ImVec2 pos, ImVec2 size) {
    if (!m_initialized || !m_imguiTextureId) return;

    // Draw the rendered HTML texture as an ImGui image
    ImGui::Image(m_imguiTextureId, size, ImVec2(0, 0), ImVec2(1, 1));
}

void HTMLMenu::OnMouseMove(int x, int y) {
    if (!m_initialized) return;
    // TODO: Forward mouse move to HTML renderer
}

void HTMLMenu::OnMouseClick(int x, int y, bool down, int button) {
    if (!m_initialized) return;
    // TODO: Forward mouse click to HTML renderer
}

void HTMLMenu::OnMouseWheel(int delta) {
    if (!m_initialized) return;
    // TODO: Forward scroll to HTML renderer
}

void HTMLMenu::OnKeyDown(int vkCode) {
    if (!m_initialized) return;
    // TODO: Forward key press to HTML renderer
}

void HTMLMenu::OnKeyUp(int vkCode) {
    if (!m_initialized) return;
    // TODO: Forward key release to HTML renderer
}

void HTMLMenu::SendMessage(const std::string& jsonMessage) {
    if (!m_initialized) return;
    // TODO: Send message to JavaScript side
}

void HTMLMenu::SetMessageCallback(std::function<void(const std::string&)> callback) {
    m_messageCallback = callback;
}
