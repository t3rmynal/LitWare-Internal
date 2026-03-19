#pragma once

typedef void (*ElectronBridgeApplyFn)(const char* key, const char* value);

// запускает websocket сервер (apply_fn может быть nullptr, задать потом через SetApply)
void ElectronBridge_Start(ElectronBridgeApplyFn apply_fn);
void ElectronBridge_SetApply(ElectronBridgeApplyFn apply_fn);

// шлёт состояние меню (открыто/закрыто)
void ElectronBridge_SendMenuOpen(bool open);

// шлёт уведомление в electron (текст)
void ElectronBridge_SendNotification(const char* text);

// поднимает electron окно поверх cs2 (вызывать из dll — foreground доступ гарантирован)
void ElectronBridge_BringToFront();

// true если foreground окно — наше electron меню
bool ElectronBridge_IsMenuFocused(void);
void ElectronBridge_SendVisibility(bool visible);

// извлекает exe из ресурса или находит рядом с dll и запускает
void ElectronBridge_LaunchMenu(void);

void ElectronBridge_Stop(void);
// закрывает electron меню (при выходе cs2)
void ElectronBridge_CloseMenu(void);
