#include "debug.h"
#include <Windows.h>
#include <cstdarg>
#include <cstdio>
#include <ctime>

// Always-on: helps diagnose injection failure (Release builds)
void BootstrapLog(const char* fmt, ...) {
    char path[MAX_PATH];
    if (GetTempPathA(MAX_PATH, path) == 0) return;
    char fullPath[MAX_PATH];
    snprintf(fullPath, sizeof(fullPath), "%slitware_dll.log", path);
    FILE* f = nullptr;
    if (fopen_s(&f, fullPath, "a") != 0 || !f) return;
    time_t t; time(&t);
    struct tm lt; if (localtime_s(&lt, &t) == 0)
        fprintf(f, "[%02d:%02d:%02d] ", lt.tm_hour, lt.tm_min, lt.tm_sec);
    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
}

void DebugLog(const char* fmt, ...) {
#ifdef LITWARE_DEBUG
    char path[MAX_PATH];
    if (GetTempPathA(MAX_PATH, path) == 0) return;

    char fullPath[MAX_PATH];
    snprintf(fullPath, sizeof(fullPath), "%slitware_dll.log", path);

    FILE* f = nullptr;
    if (fopen_s(&f, fullPath, "a") != 0 || !f) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
#else
    (void)fmt;  // no-op in Release
#endif
}
