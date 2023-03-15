
#include "battery/core/internal/platform.h"

namespace battery::internal {

#ifdef BATTERY_ARCH_WINDOWS
    HICON create_win32_icon_from_png(const battery::resource& image) {
        return CreateIconFromResourceEx(std::bit_cast<PBYTE>(image.data()), image.size(), true, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
    }
#endif // BATTERY_ARCH_WINDOWS

}