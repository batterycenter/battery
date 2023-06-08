#pragma once

#include "battery/core/string.hpp"

namespace b {

    namespace internal {
#ifdef B_OS_WINDOWS

        b::string get_last_win32_error();

#else

        void linux_run_zenity(const std::vector<b::string>& commands);

#endif
    }

    void message_box_info(const b::string& message);
    void message_box_warning(const b::string& message);
    void message_box_error(const b::string& message);

}
