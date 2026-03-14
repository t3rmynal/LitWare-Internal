#pragma once
#include <string>
#include <functional>

// Game Data Binding - serializes game state to JSON for CEF/HTML UI
class GameDataBinding {
public:
    static GameDataBinding& Get();

    // Serialize current game state to JSON (reads from g_esp_players, g_gameState)
    std::string SerializeToJson();

    // Push game data to UI (CEF or callback)
    void PushToUI();
    void SetPushCallback(std::function<void(const std::string&)> cb) { m_pushCallback = cb; }

    // Update cycle - call each frame after BuildESPData
    void Update();

private:
    GameDataBinding() = default;
    std::function<void(const std::string&)> m_pushCallback;
    std::string m_lastJson;
    uint64_t m_lastPushTick = 0;
    static constexpr uint64_t kPushIntervalMs = 100; // Throttle pushes
};
