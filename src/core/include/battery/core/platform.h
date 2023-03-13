#pragma once

#include "battery/core/environment.h"
#include <string>

namespace battery {

    // TODO: Allow to retrieve the error message in a specific language
#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error();       // This function is only available on windows, for writing the
                                              // windows-specific side of cross-platform functions
#endif

}
