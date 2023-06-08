
#include "battery/core/error.hpp"

#include <array>
#include <cstring>

namespace b {

    b::string strerror(int errnum) {
        std::array<char, 1024> buffer {};
#ifdef B_OS_WINDOWS
        return ::strerror_s(buffer.data(), buffer.size(), errnum) == 0 ? buffer.data() : b::string{};   // NOLINT
#else
        return ::strerror_r(errnum, buffer.data(), buffer.size()) == 0 ? buffer.data() : b::string{};   // NOLINT
#endif
    }

} // namespace b