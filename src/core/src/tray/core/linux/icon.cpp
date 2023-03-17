#ifndef BATTERY_CORE_NO_TRAY
#if defined(__linux__)
#include <battery/core/tray/core/icon.hpp>

Tray::Icon::Icon(std::string path) : iconPath(std::move(path)) {}
Tray::Icon::Icon(const char *path) : iconPath(path) {}
Tray::Icon::operator const char *()
{
    return iconPath.c_str();
}

#endif
#endif // BATTERY_CORE_NO_TRAY