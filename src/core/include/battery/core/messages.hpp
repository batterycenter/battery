#pragma once

#include "battery/core/string.hpp"

namespace b {

    namespace internal {
#ifdef B_OS_WINDOWS
        std::string get_last_win32_error();
#else
        void linux_run_zenity(const std::vector<std::string>& commands);
#endif
    }

    void message_box_info(const std::string& message);
    void message_box_warning(const std::string& message);
    void message_box_error(const std::string& message);

}
