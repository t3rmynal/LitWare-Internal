#pragma once

// Bootstrap log - always writes to %TEMP%\litware_dll.log (for injection diagnosis)
void BootstrapLog(const char* fmt, ...);

// Debug logging (verbose) - always declared for linker, no-op when LITWARE_DEBUG undefined
void DebugLog(const char* fmt, ...);
