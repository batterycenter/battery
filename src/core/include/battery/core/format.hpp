#ifndef BATTERY_FORMAT_HPP
#define BATTERY_FORMAT_HPP

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "battery/core/string.hpp"
#include <spdlog/fmt/fmt.h>                     // TODO: Use std::format once it's fully implemented

namespace b {

    template<typename... T> b::string format(fmt::format_string<T...> fmt, T&&... args) {
        return b::string::decode_utf8(fmt::format(fmt, std::forward<T>(args)...));
    }

} // namespace b

#endif //BATTERY_FORMAT_HPP
