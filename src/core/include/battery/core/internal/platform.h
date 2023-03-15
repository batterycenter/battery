#pragma once

#include "battery/core/environment.h"
#include "battery/core/resource.h"

#ifdef BATTERY_ARCH_WINDOWS
#include "battery/core/internal/windows.h"
#endif // BATTERY_ARCH_WINDOWS

namespace battery::internal {

#ifdef BATTERY_ARCH_WINDOWS
    HICON create_win32_icon_from_png(const battery::resource& image);
#endif // BATTERY_ARCH_WINDOWS

}
