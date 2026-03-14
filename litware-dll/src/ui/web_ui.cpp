#include "web_ui.h"
#include <iostream>

WebUIBridge& WebUIBridge::Get() {
    static WebUIBridge instance;
    return instance;
}

void WebUIBridge::Initialize(std::function<void(const json&)> onUIMessage) {
    m_onUIMessage = onUIMessage;
}

void WebUIBridge::SendMessage(const std::string& type, const json& data) {
    json message = data;
    message["type"] = type;

    // In a real CEF implementation, this would call JavaScript:
    // browser->ExecuteFunction("window.menuController.handleCppMessage", message.dump());

    std::cout << "[WebUI] Sending to JS: " << message.dump() << std::endl;
}

void WebUIBridge::ProcessMessage(const std::string& jsonString) {
    try {
        json message = json::parse(jsonString);
        std::string type = message.value("type", "");

        // Handle specific message types
        if (type == "control_changed") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "keybind_set") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "tab_changed") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "theme_changed") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "save_config") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "load_config") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (type == "reset_config") {
            if (m_onUIMessage) m_onUIMessage(message);
        }
        else if (m_commandHandlers.find(type) != m_commandHandlers.end()) {
            auto result = m_commandHandlers[type](message);
            // Send result back if needed
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[WebUI] Parse error: " << e.what() << std::endl;
    }
}

void WebUIBridge::RegisterCommand(const std::string& command, std::function<json(const json&)> handler) {
    m_commandHandlers[command] = handler;
}

// HTML Menu UI Implementation
HTMLMenuUI::HTMLMenuUI() {
    RegisterDefaultHandlers();
}

HTMLMenuUI::~HTMLMenuUI() = default;

bool HTMLMenuUI::LoadHTML(const std::string& htmlPath) {
    // Initialize WebUI bridge
    WebUIBridge::Get().Initialize([this](const json& msg) {
        this->OnUIMessage(msg);
    });

    m_initialized = true;
    return true;
}

void HTMLMenuUI::Update() {
    if (!m_initialized) return;

    // Update UI with current game state
    // This is called from the render loop
}

void HTMLMenuUI::SendUIMessage(const std::string& type, const json& data) {
    WebUIBridge::Get().SendMessage(type, data);
}

void HTMLMenuUI::OnUIMessage(const json& message) {
    std::string type = message.value("type", "");

    if (type == "control_changed") {
        std::string id = message.value("id", "");
        auto value = message["value"];
        UpdateConfig({{ id, value }});
    }
    else if (type == "keybind_set") {
        std::string id = message.value("id", "");
        std::string key = message.value("key", "");
        m_config[id] = key;
    }
    else if (type == "theme_changed") {
        int theme = message.value("theme", 0);
        m_config["theme"] = theme;
    }
    else if (type == "save_config") {
        std::string name = message.value("name", "default");
        auto config = message.value("config", json::object());
        m_config.update(config);
        // Save to file
    }
}

void HTMLMenuUI::UpdateConfig(const json& updates) {
    m_config.update(updates);
}

void HTMLMenuUI::RegisterDefaultHandlers() {
    WebUIBridge& bridge = WebUIBridge::Get();

    bridge.RegisterCommand("get_players", [](const json& msg) {
        return json{ {"count", 0} };
    });

    bridge.RegisterCommand("get_status", [](const json& msg) {
        return json{
            {"esp", true},
            {"hook", true},
            {"players", 0}
        };
    });
}
