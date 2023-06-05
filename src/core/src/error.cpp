
#include "battery/core/error.hpp"
#include "battery/core/environment.hpp"
#include <array>

namespace b {

    b::string strerror(int errnum) {
        std::array<char, 1024> buffer {};
#ifdef BATTERY_ARCH_WINDOWS
        return ::strerror_s(buffer.data(), buffer.size(), errnum) == 0 ? buffer.data() : b::string();
#else
        return ::strerror_r(errnum, buffer.data(), buffer.size()) == 0 ? buffer.data() : b::string();
#endif
    }

} // namespace b