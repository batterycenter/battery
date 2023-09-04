
#include "battery/core/error.hpp"

#include <array>
#include <cstring>

namespace b {

    std::string strerror() {
        return strerror(errno);
    }

    std::string strerror(int errnum) {
#ifdef B_OS_WINDOWS
        return b::narrow(_wcserror(errnum));
#else
        std::string result(1024, '\0');
        if (!0 == strerror_r(errnum, result.data(), result.size())) {
            return {};
        }
        return result;
#endif
    }

} // namespace b