#pragma once

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include <variant>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <spdlog/fmt/fmt.h>                     // TODO: Use std::format once it's fully implemented

#include "battery/core/internal/extern/rang.hpp"

namespace b::log {

    namespace internal {
        inline std::shared_ptr<spdlog::logger> core_logger;     // Core is for battery internal log messages
        inline std::shared_ptr<spdlog::logger> user_logger;     // User is for user level log messages
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
        if (!internal::core_logger || !internal::user_logger) {
            internal::core_logger = spdlog::stdout_color_mt("battery::core");
            internal::core_logger->set_level((spdlog::level::level_enum)level::DEBUG);
            internal::core_logger->set_pattern("%^[%T] [%n] %v%$");

            internal::user_logger = spdlog::stdout_color_mt("battery::user");
            internal::user_logger->set_level((spdlog::level::level_enum)level::DEBUG);
            internal::user_logger->set_pattern("%^[%T] %v%$");
        }
    }

    inline void level(enum log::level level) {
        internal::user_logger->set_level((spdlog::level::level_enum)level);
    }

    inline void pattern(const std::string& pattern) {
        internal::user_logger->set_pattern(pattern);
    }

    template<typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->trace(fmt, std::forward<T>(args)...); }
    template<typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->debug(fmt, std::forward<T>(args)...); }
    template<typename... T> auto info(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->info(fmt, std::forward<T>(args)...); }
    template<typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->warn(fmt, std::forward<T>(args)...); }
    template<typename... T> auto error(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->error(fmt, std::forward<T>(args)...); }
    template<typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::user_logger->critical(fmt, std::forward<T>(args)...); }




    namespace core {

        inline void level(enum level level) {
            internal::core_logger->set_level((spdlog::level::level_enum)level);
        }

        inline void pattern(const std::string& pattern) {
            internal::core_logger->set_pattern(pattern);
        }

        template<typename... T> auto trace(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->trace(fmt, std::forward<T>(args)...); }
        template<typename... T> auto debug(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->debug(fmt, std::forward<T>(args)...); }
        template<typename... T> auto info(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->info(fmt, std::forward<T>(args)...); }
        template<typename... T> auto warn(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->warn(fmt, std::forward<T>(args)...); }
        template<typename... T> auto error(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->error(fmt, std::forward<T>(args)...); }
        template<typename... T> auto critical(fmt::format_string<T...> fmt, T&&... args) { __init(); internal::core_logger->critical(fmt, std::forward<T>(args)...); }

    }

}

namespace b {

    namespace colors {          // Re-import rang as a b::colors namespace
        using namespace rang;
    }

    using b_color_variants_t = std::variant<b::colors::fg, b::colors::bg, b::colors::bgB, b::colors::bgB, b::colors::style>;
    void print(const std::string& str);
    void print(const b_color_variants_t& color, const std::string& str);
    void print(const std::vector<b_color_variants_t>& color, const std::string& str);

    template<typename... T>
    void print(fmt::format_string<T...> fmt, T&&... args) {
        b::print(fmt::format(fmt, std::forward<T>(args)...));
    }

    template<typename... T>
    void print(const b_color_variants_t& color, fmt::format_string<T...> fmt, T&&... args) {
        b::print(color, fmt::format(fmt, std::forward<T>(args)...));
    }

    template<typename... T>
    void print(const std::vector<b_color_variants_t>& color, fmt::format_string<T...> fmt, T&&... args) {
        b::print(color, fmt::format(fmt, std::forward<T>(args)...));
    }

}
