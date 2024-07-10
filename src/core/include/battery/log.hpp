#pragma once

#pragma GCC diagnostic push // Ignore deprecation warnings from 'fmt' library
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "format.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <variant>

#pragma GCC diagnostic pop

#ifdef ERROR // An 'ERROR' macro is leaked somewhere from some windows header, and it
             // conflicts with our enums
#undef ERROR
#endif

#include "battery/internal/extern/rang.hpp"
#include "string.hpp"

namespace b::log {

namespace internal {
    inline std::shared_ptr<spdlog::logger>
        coreLogger; // Core is for battery internal log messages
    inline std::shared_ptr<spdlog::logger>
        userLogger; // User is for user level log messages
}

enum class level {
    TRACE = SPDLOG_LEVEL_TRACE,
    DEBUG = SPDLOG_LEVEL_DEBUG,
    INFO = SPDLOG_LEVEL_INFO,
    WARN = SPDLOG_LEVEL_WARN,
    ERROR = SPDLOG_LEVEL_ERROR,
    CRITICAL = SPDLOG_LEVEL_CRITICAL,
    OFF = SPDLOG_LEVEL_OFF
};

inline void __init()
{
    if (!internal::coreLogger || !internal::userLogger) {
        internal::coreLogger = spdlog::stdout_color_mt("battery::core");
        internal::coreLogger->set_level(
            static_cast<spdlog::level::level_enum>(level::DEBUG));
        internal::coreLogger->set_pattern("%^[%T] [%n] %v%$");

        internal::userLogger = spdlog::stdout_color_mt("battery::user");
        internal::userLogger->set_level(
            static_cast<spdlog::level::level_enum>(level::DEBUG));
        internal::userLogger->set_pattern("%^[%T] %v%$");
    }
}

inline void level(enum log::level level)
{
    internal::userLogger->set_level(static_cast<spdlog::level::level_enum>(level));
}

inline void pattern(const std::string& pattern)
{
    internal::userLogger->set_pattern(pattern);
}

template <typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->trace(fmt, std::forward<T>(args)...);
}
template <typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->debug(fmt, std::forward<T>(args)...);
}
template <typename... T> auto info(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->info(fmt, std::forward<T>(args)...);
}
template <typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->warn(fmt, std::forward<T>(args)...);
}
template <typename... T> auto error(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->error(fmt, std::forward<T>(args)...);
}
template <typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args)
{
    __init();
    internal::userLogger->critical(fmt, std::forward<T>(args)...);
}

namespace core {

    inline void level(enum level level)
    {
        internal::coreLogger->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    inline void pattern(const std::string& pattern)
    {
        internal::coreLogger->set_pattern(pattern);
    }

    template <typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->trace(fmt, std::forward<T>(args)...);
    }
    template <typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->debug(fmt, std::forward<T>(args)...);
    }
    template <typename... T> auto info(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->info(fmt, std::forward<T>(args)...);
    }
    template <typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->warn(fmt, std::forward<T>(args)...);
    }
    template <typename... T> auto error(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->error(fmt, std::forward<T>(args)...);
    }
    template <typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args)
    {
        __init();
        internal::coreLogger->critical(fmt, std::forward<T>(args)...);
    }

}

} // namespace b::log

namespace b {

/// \brief Print a formatted string using Python formatting syntax
/// \details This function uses fmt::print under the hood. See
/// https://fmt.dev/latest/syntax.html for more info.
///          Example usage: `b::print("Hello, {0}!", "world")`. The format string must be
///          a plain string literal, the arguments can be any type that provides a
///          formatter for fmt::format. Custom types can be formatted by providing a
///          specialization of fmt::formatter. See
///          https://fmt.dev/latest/api.html#formatting-user-defined-types
/// \param fmt The format string
/// \param args The arguments to format
using fmt::print;

template <typename... T> void println(fmt::format_string<T...> fmt, T&&... args)
{
    fmt::print(fmt, std::forward<T>(args)...);
    fmt::print("\n");
}

} // namespace b
