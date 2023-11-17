#pragma once

#include "string.hpp"

namespace b {

    enum class Platform {
        Windows,
        Linux,
        MacOS,
        Web
    };

    enum class Arch {
        x64,
        x86,
    };

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace Constants {
        std::string BatteryIconBase64();
        std::string DefaultApplicationName();

        bool MessageBoxOnExceptionDefault();
        bool CatchCommonExceptionsDefault();
        Platform Platform();
        Arch Architecture();

        double Pi();

        bool IsProductionMode();
    } // namespace Constants

} // namespace b
