#pragma once

namespace bypass {

// Initialize anti-cheat bypass (BSecureAllowed hook).
// Call early in entry(), before render hook if possible.
bool Initialize();

// Optional: force g_bClientIsntAllowedToPlayOnSecureServers = 0 (allow secure servers).
// Uses pattern scan in client.dll - call after game loaded.
void PatchSecureServerFlag();

} // namespace bypass
