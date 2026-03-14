# LitWare DLL

Internal DLL for Counter-Strike 2. Injects into `cs2.exe`, hooks DirectX 11 Present, renders ImGui overlay in-game.

## Build

1. Open `litware-dll.vcxproj` in Visual Studio 2022
2. Select **Release | x64** (or Debug)
3. Output: `bin/Release/litware-dll.dll` or `bin/Debug/`
4. Inject into `cs2.exe` after main menu loads

## Dependencies (in repo)

- `external/imgui` — ImGui (bundled)
- `external/minhook` — MinHook (bundled)
- `../vendor/omath` — math utilities

## Controls

- **INSERT** — Toggle menu
- **F1** — Keybinds
- **END** — Unload

## Troubleshooting

1. **Debug build** — logs to `%TEMP%\litware_dll.log`
2. **Steam overlay** must be enabled
3. Inject **after** CS2 main menu loads
