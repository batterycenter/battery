#pragma once

#include "battery/core/string.hpp"

namespace b {

    enum class platform_t {
        windows,
        linux,
        macos,
    };

    enum class arch_t {
        x64,
        x86,
    };

    // These constants are defined as static getter functions instead of variables,
    // to avoid a 'static initialization order fiasco' https://isocpp.org/wiki/faq/ctors#static-init-order
    struct constants {
        static b::string battery_icon_base64();
        static b::string default_application_name();

        static bool message_box_on_exception_default();
        static bool catch_common_exceptions_default();
        static platform_t platform();
        static arch_t architecture();

        static bool is_production_mode();
    };

} // namespace b
