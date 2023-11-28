#pragma once

#include "string.hpp"
#include "vec.hpp"
#include "imgui.h"

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
        std::string BatteryIconBase64() noexcept;
        std::string DefaultApplicationName() noexcept;
        double AnimationHeartbeatInterval() noexcept;
        double BlinkingCursorHeartbeatInterval() noexcept;

        bool MessageBoxOnExceptionDefault() noexcept;
        bool CatchCommonExceptionsDefault() noexcept;
        Platform Platform() noexcept;
        Arch Architecture() noexcept;

        double Pi() noexcept;

        bool IsProductionMode() noexcept;

        ImColor DefaultWindowBackgroundColor() noexcept;
        ImColor DefaultWindowTitlebarColor() noexcept;
        ImColor DefaultWindowFrameColor() noexcept;
        double DefaultFontSize() noexcept;
        b::Vec2 DefaultWindowSize() noexcept;
        std::string DefaultWindowTitle() noexcept;

    } // namespace Constants

} // namespace b
