#pragma once

#include "battery/core/environment.hpp"
#include <string>

namespace b {

    namespace internal {
#ifdef BATTERY_ARCH_WINDOWS

        std::string get_last_win32_error();

#else

        void linux_run_zenity(const std::vector<std::string>& commands);

#endif
    }

    void message_box_info(const std::string& message);
    void message_box_info(const std::u8string& message);
    void message_box_info(const std::u32string& message);

    void message_box_warning(const std::string& message);
    void message_box_warning(const std::u8string& message);
    void message_box_warning(const std::u32string& message);

    void message_box_error(const std::string& message);
    void message_box_error(const std::u8string& message);
    void message_box_error(const std::u32string& message);

}
