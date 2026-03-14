# LitWare CS2 Internal

**Internal cheat for Counter-Strike 2.** Injects into `cs2.exe`, hooks DirectX 11 Present, renders an ImGui overlay directly in-game — no external overlay, no input lag.

| | |
|---|---|
| **Platform** | Windows x64 |
| **Engine** | Source 2 |
| **Build** | Visual Studio 2022 |
| **Dependencies** | Steam (gameoverlayrenderer64.dll), ImGui, MinHook |

---

## GitHub Setup

1. **Initialize Git** (if not already):
   ```bash
   git init
   ```

2. **Add files and commit**:
   ```bash
   git add .
   git commit -m "Initial commit: LitWare CS2 Internal"
   ```

3. **Create a new repository** on GitHub (e.g. `cs2-litware-internal`) — do **not** add a README or .gitignore.

4. **Push to GitHub**:
   ```bash
   git remote add origin https://github.com/YOUR_USERNAME/cs2-litware-internal.git
   git branch -M main
   git push -u origin main
   ```

5. **Private repo** recommended — public cheat repos can attract attention.

| | |
|---|---|
| **Platform** | Windows x64 |
| **Engine** | Source 2 |
| **Build** | Visual Studio 2022 |
| **Dependencies** | Steam (gameoverlayrenderer64.dll), ImGui, MinHook |

---

## Features

| Category | Features |
|----------|----------|
| **ESP** | Boxes (corner/rounded/sharp), skeleton, health bar, names, weapons, ammo, money, distance, offscreen arrows |
| **Aimbot** | FOV, smoothing, bone selection, team check |
| **Visuals** | No flash, no smoke, glow, chams (enemy/team/ignore-z), world/sky color, snow, sakura |
| **Movement** | Bunny hop, strafe helper, anti-aim (spin/desync/jitter) |
| **Misc** | Third person, FOV changer, radar, bomb timer, spectator list |
| **Config** | Save/load to `%APPDATA%\litware\` |

---

## Controls

| Key | Action |
|-----|--------|
| **INSERT** | Toggle menu |
| **F1** | Keybinds window |
| **END** | Unload DLL |

---

## Build

### Prerequisites

- Visual Studio 2022 (x64 workload)
- Steam running (required for `gameoverlayrenderer64.dll`)
- CS2 installed

### Steps

1. Open `litware-dll/litware-dll.vcxproj` in Visual Studio (or `litware-dll.sln` at root)
2. Select **Release | x64** (or Debug for logs)
3. Build → `litware-dll/bin/Release/litware-dll.dll`
4. Inject into `cs2.exe` after the main menu loads

### Dependencies (in repo)

- `litware-dll/external/imgui` — ImGui (bundled)
- `litware-dll/external/minhook` — MinHook (bundled)
- `vendor/omath` — math utilities
- `icons/CS2GunIcons.ttf` — weapon icons

---

## Offsets

Offsets are kept in `litware-dll/src/core/offsets.h`, synced with [cs2-dumper](https://github.com/a2x/cs2-dumper).

After game updates: run cs2-dumper, refresh `offsets/output/`, and update `offsets.h`.

---

## Project Structure

```
├── litware-dll/            # Main DLL (Litware)
│   ├── src/                # Source
│   │   ├── hooks/          # render_hook, Present hook
│   │   ├── core/           # offsets, entity, esp_data
│   │   └── ...
│   ├── external/          # ImGui, MinHook (bundled)
│   └── res/                # Menu assets
├── offsets/                # cs2-dumper output
├── vendor/omath/           # Math library
└── icons/                  # Fonts
```

---

## License

GPL-3.0 — see [LICENSE](LICENSE).

---

## Disclaimer

Educational use only. Use at your own risk. VAC bans possible.
