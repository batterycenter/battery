#pragma once

#include "battery/core/format.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <variant>

#ifdef ERROR    // An 'ERROR' macro is leaked somewhere from some windows header, and it conflicts with our enums
#undef ERROR
#endif

#include "battery/core/internal/extern/rang.hpp"
#include "battery/core/string.hpp"

namespace b::log {

    namespace internal {
        inline std::shared_ptr<spdlog::logger> coreLogger;     // Core is for battery internal log messages
        inline std::shared_ptr<spdlog::logger> userLogger;     // User is for user level log messages
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

    inline void __init() {
        if (!internal::coreLogger || !internal::userLogger) {
            internal::coreLogger = spdlog::stdout_color_mt("battery::core");
            internal::coreLogger->set_level(static_cast<spdlog::level::level_enum>(level::DEBUG));
            internal::coreLogger->set_pattern("%^[%T] [%n] %v%$");

            internal::userLogger = spdlog::stdout_color_mt("battery::user");
            internal::userLogger->set_level(static_cast<spdlog::level::level_enum>(level::DEBUG));
            internal::userLogger->set_pattern("%^[%T] %v%$");
        }
    }

    inline void level(enum log::level level) {
        internal::userLogger->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    inline void pattern(const b::string& pattern) {
        internal::userLogger->set_pattern(pattern.encode<b::enc::utf8>());
    }

    template<typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->trace(fmt, std::forward<T>(args)...); }
    template<typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->debug(fmt, std::forward<T>(args)...); }
    template<typename... T> auto info(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->info(fmt, std::forward<T>(args)...); }
    template<typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->warn(fmt, std::forward<T>(args)...); }
    template<typename... T> auto error(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->error(fmt, std::forward<T>(args)...); }
    template<typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::userLogger->critical(fmt, std::forward<T>(args)...); }

    template<typename... T> auto trace(const b::string& str) { trace("{}", str); }
    template<typename... T> auto debug(const b::string& str) { debug("{}", str); }
    template<typename... T> auto info(const b::string& str) { info("{}", str); }
    template<typename... T> auto warn(const b::string& str) { warn("{}", str); }
    template<typename... T> auto error(const b::string& str) { error("{}", str); }
    template<typename... T> auto critical(const b::string& str) { critical("{}", str); }

    template<typename... T> auto trace(const std::string& str) { trace("{}", str); }
    template<typename... T> auto debug(const std::string& str) { debug("{}", str); }
    template<typename... T> auto info(const std::string& str) { info("{}", str); }
    template<typename... T> auto warn(const std::string& str) { warn("{}", str); }
    template<typename... T> auto error(const std::string& str) { error("{}", str); }
    template<typename... T> auto critical(const std::string& str) { critical("{}", str); }

    namespace core {

        inline void level(enum level level) {
            internal::coreLogger->set_level(static_cast<spdlog::level::level_enum>(level));
        }

        inline void pattern(const b::string& pattern) {
            internal::coreLogger->set_pattern(pattern.encode<b::enc::utf8>());
        }

        template<typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->trace(fmt, std::forward<T>(args)...); }
        template<typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->debug(fmt, std::forward<T>(args)...); }
        template<typename... T> auto info(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->info(fmt, std::forward<T>(args)...); }
        template<typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->warn(fmt, std::forward<T>(args)...); }
        template<typename... T> auto error(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->error(fmt, std::forward<T>(args)...); }
        template<typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::coreLogger->critical(fmt, std::forward<T>(args)...); }

        template<typename... T> auto trace(const b::string& str) { trace("{}", str); }
        template<typename... T> auto debug(const b::string& str) { debug("{}", str); }
        template<typename... T> auto info(const b::string& str) { info("{}", str); }
        template<typename... T> auto warn(const b::string& str) { warn("{}", str); }
        template<typename... T> auto error(const b::string& str) { error("{}", str); }
        template<typename... T> auto critical(const b::string& str) { critical("{}", str); }

        template<typename... T> auto trace(const std::string& str) { trace("{}", str); }
        template<typename... T> auto debug(const std::string& str) { debug("{}", str); }
        template<typename... T> auto info(const std::string& str) { info("{}", str); }
        template<typename... T> auto warn(const std::string& str) { warn("{}", str); }
        template<typename... T> auto error(const std::string& str) { error("{}", str); }
        template<typename... T> auto critical(const std::string& str) { critical("{}", str); }

    }

} // namespace b::log

namespace b {

    namespace colors {          // Re-import rang as a b::colors namespace
        using namespace rang;
    } // namespace colors

    using b_color_variants_t = std::variant<b::colors::fg, b::colors::bg, b::colors::bgB, b::colors::bgB, b::colors::style>;
    void print(const std::string& str);
    void print(const b::string& str);
    void print(const b_color_variants_t& color, const b::string& str);
    void print(const std::vector<b_color_variants_t>& color, const b::string& str);

    template<typename... T>
    void print(fmt::format_string<T...> fmt, T&&... args) {
        b::print(b::format(fmt, std::forward<T>(args)...));
    }

    template<typename... T>
    void print(const b_color_variants_t& color, fmt::format_string<T...> fmt, T&&... args) {
        b::print(color, b::format(fmt, std::forward<T>(args)...));
    }

    template<typename... T>
    void print(const std::vector<b_color_variants_t>& color, fmt::format_string<T...> fmt, T&&... args) {
        b::print(color, b::format(fmt, std::forward<T>(args)...));
    }

} // namespace b