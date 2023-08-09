
#include "battery/core/error.hpp"

#include <array>
#include <cstring>

namespace b {

    b::string strerror() {
        return strerror(errno);
    }

    b::string strerror(int errnum) {
#ifdef B_OS_WINDOWS
        b::native_string const result = _wcserror(errnum);
#else
        std::string result(1024, '\0');
        if (!0 == strerror_r(errnum, result.data(), result.size())) {
            return {};
        }
#endif
        return b::string::decode_native(result);
    }

} // namespace b