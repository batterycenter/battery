#ifndef BATTERY_CORE_NO_TRAY
#if defined(_WIN32)
#include <battery/core/tray/core/icon.hpp>
#include <battery/core/log.h>

static HICON loadIcon(const battery::resource& icon) {
    return CreateIconFromResourceEx(std::bit_cast<PBYTE>(icon.data()), icon.size(), true, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
}

Tray::Icon::Icon(const battery::resource& icon) : hIcon(loadIcon(icon)) {}
Tray::Icon::Icon(HICON icon) : hIcon(icon) {}
Tray::Icon::Icon(WORD icon) : hIcon(LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(icon))) {} // NOLINT

Tray::Icon::operator HICON() {
    return hIcon;
}

#endif
#endif // BATTERY_CORE_NO_TRAY