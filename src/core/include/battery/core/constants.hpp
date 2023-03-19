#pragma once

#include <string>

namespace b {

    // These constants are defined as static getter functions to avoid a 'static initialization order fiasco'
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    struct constants {
        static std::string battery_icon_base64();
        static std::string default_application_name();

        static bool message_box_on_exception_default();
        static bool catch_common_exceptions_default();
    };

}
