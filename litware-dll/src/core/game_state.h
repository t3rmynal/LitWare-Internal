#pragma once
#include <cstdint>

// Game state snapshot - populated each frame by render_hook for CEF/UI binding
struct GameState {
    bool bombActive = false;
    int bombSite = -1;
    float bombExplodeTime = 0.f;
    bool bombDefusing = false;
    float bombDefuseEnd = 0.f;
    float gameTime = 0.f;
};
inline GameState g_gameState = {};
