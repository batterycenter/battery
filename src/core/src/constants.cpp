
#include "battery/core/constants.hpp"

namespace b {

    b::string Constants::BatteryIconBase64() {
        return "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";
    }

    b::string Constants::DefaultApplicationName() {
        return "BatteryApp";
    }

    Platform Constants::Platform() {
#ifdef B_OS_WINDOWS
        return Platform::Windows;
#elif defined(B_OS_LINUX)
        return Platform::Linux;
#elif defined(B_OS_MACOS)
        return Platform::MacOS;
#elif defined(B_OS_WEB)
        return Platform::Web;
#else
        #error "Unknown platform"
#endif
    }

    Arch Constants::Architecture() {
#ifdef B_ARCH_64
        return Arch::x64;
#elif defined(B_ARCH_32)
        return Arch::x86;
#else
        #error "Invalid architecure configuration!"
#endif
    }

    double Constants::PI() {
        return 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
    }

    float Constants::fPI() {
        return static_cast<float>(PI());
    }

#ifdef B_DEBUG
    bool Constants::MessageBoxOnExceptionDefault() { return true; }
    bool Constants::CatchCommonExceptionsDefault() { return true; }
#else
    bool Constants::MessageBoxOnExceptionDefault() { return false; }
    bool Constants::CatchCommonExceptionsDefault() { return true; }
#endif

    bool Constants::IsProductionMode() {
#ifdef B_PRODUCTION_MODE
        return true;
#else
        return false;
#endif
    }

}  // namespace b
