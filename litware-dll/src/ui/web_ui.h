#pragma once
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Lightweight Web UI Bridge for communicating with HTML/CEF
class WebUIBridge {
public:
    static WebUIBridge& Get();

    // Initialize bridge with callback for C++
    void Initialize(std::function<void(const json&)> onUIMessage);

    // Send message from C++ to UI (JavaScript)
    void SendMessage(const std::string& type, const json& data = {});

    // Process message from UI (JavaScript) to C++
    void ProcessMessage(const std::string& jsonString);

    // Register command handler
    void RegisterCommand(const std::string& command, std::function<json(const json&)> handler);

private:
    WebUIBridge() = default;
    ~WebUIBridge() = default;

    std::function<void(const json&)> m_onUIMessage;
    std::unordered_map<std::string, std::function<json(const json&)>> m_commandHandlers;
};

// HTML Menu Integration
class HTMLMenuUI {
public:
    HTMLMenuUI();
    ~HTMLMenuUI();

    bool LoadHTML(const std::string& htmlPath);
    void Update();

    // Bridge methods
    void SendUIMessage(const std::string& type, const json& data = {});
    void OnUIMessage(const json& message);

    // Get config state
    const json& GetConfig() const { return m_config; }
    void UpdateConfig(const json& updates);

private:
    json m_config;
    bool m_initialized = false;

    void RegisterDefaultHandlers();
};
