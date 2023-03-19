#pragma once

#include "battery/core/environment.h"
#include <string>

namespace battery {

#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error();
#endif

    void message_box_info(const std::string& message);
    void message_box_warning(const std::string& message);
    void message_box_error(const std::string& message);

}
