#pragma once

#include "battery/core/string.hpp"

namespace b {

    enum class Platform {
        Windows,
        Linux,
        MacOS,
    };

    enum class Arch {
        x64,
        x86,
    };

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace Constants {
        b::string BatteryIconBase64();
        b::string DefaultApplicationName();

        bool MessageBoxOnExceptionDefault();
        bool CatchCommonExceptionsDefault();
        Platform Platform();
        Arch Architecture();

        double PI();
        float fPI();

        bool IsProductionMode();
    } // namespace Constants

} // namespace b
