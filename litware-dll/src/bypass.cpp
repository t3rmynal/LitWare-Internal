// Anti-cheat bypass based on bypass-learn.txt (CS2 diagnostic system analysis)
// Bypasses: BSecureAllowed check, g_bClientIsntAllowedToPlayOnSecureServers
#include "bypass.h"
#include "debug.h"
#include <Windows.h>
#include <MinHook.h>
#include <cstdint>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

namespace {

// BSecureAllowed(char* report_buf, int buf_size, int flags)
// Returns: 0 = violations / block, non-zero = allowed
// We hook to always return 1 (allowed)
using BSecureAllowedFn = int (*)(char*, int, int);
static BSecureAllowedFn g_origBSecureAllowed = nullptr;

static int HookBSecureAllowed(char* report_buf, int buf_size, int flags) {
    (void)report_buf;
    (void)buf_size;
    (void)flags;
    // Always report "allowed" - no violations
    return 1;
}

static void* PatternScan(HMODULE mod, const char* pat, const char* mask) {
    if (!mod || !pat || !mask) return nullptr;
    MODULEINFO mi{};
    if (!GetModuleInformation(GetCurrentProcess(), mod, &mi, sizeof(mi))) return nullptr;
    auto* b = static_cast<const uint8_t*>(mi.lpBaseOfDll);
    const size_t sz = mi.SizeOfImage;
    const size_t pl = strlen(mask);
    for (size_t i = 0; i + pl <= sz; ++i) {
        bool ok = true;
        for (size_t j = 0; j < pl && ok; ++j)
            if (mask[j] != '?' && b[i + j] != static_cast<uint8_t>(pat[j]))
                ok = false;
        if (ok) return const_cast<void*>(static_cast<const void*>(b + i));
    }
    return nullptr;
}

// g_bClientIsntAllowedToPlayOnSecureServers at client.dll+0x1CFB788 (build-specific)
// Pattern: look for LEA/MOV that references this global, or CMP with it
// Common pattern: "38 1D ?? ?? ?? ??" (cmp byte ptr [rip+offset], bl) or similar
// We search for instruction that writes 1 to this address
static bool g_secureFlagPatched = false;

} // namespace

namespace bypass {

bool Initialize() {
    // BSecureAllowed is exported from cs2.exe (main process exe)
    HMODULE exe = GetModuleHandleA(nullptr);
    if (!exe) {
        exe = GetModuleHandleA("cs2.exe");
    }
    if (!exe) {
        DebugLog("[bypass] cs2.exe not found");
        return false;
    }

    void* pBSecureAllowed = GetProcAddress(exe, "BSecureAllowed");
    if (!pBSecureAllowed) {
        DebugLog("[bypass] BSecureAllowed export not found");
        return false;
    }

    if (MH_Initialize() != MH_OK) {
        DebugLog("[bypass] MH_Initialize failed (may already be initialized)");
        // MinHook might already be init'd by render_hook - try hook anyway
    }

    MH_STATUS st = MH_CreateHook(pBSecureAllowed, &HookBSecureAllowed,
        reinterpret_cast<void**>(&g_origBSecureAllowed));
    if (st != MH_OK) {
        DebugLog("[bypass] MH_CreateHook BSecureAllowed failed: %d", static_cast<int>(st));
        return false;
    }

    if (MH_EnableHook(pBSecureAllowed) != MH_OK) {
        DebugLog("[bypass] MH_EnableHook BSecureAllowed failed");
        return false;
    }

    DebugLog("[bypass] BSecureAllowed hooked OK");
    return true;
}

void PatchSecureServerFlag() {
    if (g_secureFlagPatched) return;

    HMODULE client = GetModuleHandleA("client.dll");
    if (!client) return;

    // Pattern: "C6 05 ?? ?? ?? ?? 01" = mov byte ptr [rip+rel32], 1
    // Patches the imm8 (last byte) from 1 to 0 so it writes 0 instead
    static const char PAT[] = "\xC6\x05\x00\x00\x00\x00\x01";
    static const char MSK[] = "xx????x";
    void* hit = PatternScan(client, PAT, MSK);
    if (!hit) {
        return;  // Pattern may vary by build; BSecureAllowed hook is primary
    }

    __try {
        DWORD oldProt;
        uint8_t* instr = static_cast<uint8_t*>(hit);
        if (VirtualProtect(instr, 7, PAGE_EXECUTE_READWRITE, &oldProt)) {
            instr[6] = 0;  // Change mov ..., 1 to mov ..., 0
            VirtualProtect(instr, 7, oldProt, &oldProt);
            g_secureFlagPatched = true;
            DebugLog("[bypass] secure-server write patched (mov 1 -> 0)");
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        DebugLog("[bypass] PatchSecureServerFlag: exception");
    }
}

} // namespace bypass
