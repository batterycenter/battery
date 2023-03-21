#pragma once

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <spdlog/fmt/fmt.h>                     // TODO: Use std::format once it's fully implemented

namespace b::log {

    namespace internal {
        inline std::shared_ptr<spdlog::logger> core_logger;     // Core is for battery internal log messages
        inline std::shared_ptr<spdlog::logger> user_logger;     // User is for user level log messages (diagnostics only)
        inline std::shared_ptr<spdlog::logger> print_logger;    // For b::print, always printed and without patterns
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
        if (!internal::core_logger || !internal::user_logger || !internal::print_logger) {
            internal::core_logger = spdlog::stdout_color_mt("battery::core");
            internal::core_logger->set_level((spdlog::level::level_enum)level::DEBUG);
            internal::core_logger->set_pattern("%^[%T] [%n] %v%$");

            internal::user_logger = spdlog::stdout_color_mt("battery::user");
            internal::user_logger->set_level((spdlog::level::level_enum)level::DEBUG);
            internal::user_logger->set_pattern("%^[%T] %v%$");

            internal::print_logger = spdlog::stdout_color_mt("battery::print");
            internal::print_logger->set_level((spdlog::level::level_enum)level::TRACE);
            internal::print_logger->set_pattern("%v%$");
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

        inline void level(enum Level level) {
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

    enum class print_color {
        WHITE = SPDLOG_LEVEL_TRACE,
        BLUE = SPDLOG_LEVEL_DEBUG,
        GREEN = SPDLOG_LEVEL_INFO,
        YELLOW = SPDLOG_LEVEL_WARN,
        RED = SPDLOG_LEVEL_ERROR,
        INTENSE_RED = SPDLOG_LEVEL_CRITICAL
    };

    template<typename... T>
    auto print(fmt::format_string<T...> fmt, T&&... args) {
        b::log::__init();
        b::log::internal::print_logger->trace(fmt, std::forward<T>(args)...);
    }

    template<typename... T>
    auto print(enum b::print_color color, fmt::format_string<T...> fmt, T&&... args) {
        b::log::__init();
        switch (color) {
            case b::print_color::WHITE:
                b::log::internal::print_logger->trace(fmt, std::forward<T>(args)...);
                break;
            case b::print_color::BLUE:
                b::log::internal::print_logger->debug(fmt, std::forward<T>(args)...);
                break;
            case b::print_color::GREEN:
                b::log::internal::print_logger->info(fmt, std::forward<T>(args)...);
                break;
            case b::print_color::YELLOW:
                b::log::internal::print_logger->warn(fmt, std::forward<T>(args)...);
                break;
            case b::print_color::RED:
                b::log::internal::print_logger->error(fmt, std::forward<T>(args)...);
                break;
            case b::print_color::INTENSE_RED:
                b::log::internal::print_logger->critical(fmt, std::forward<T>(args)...);
                break;
            default:
                break;
        }
    }

    inline void print_pattern(const std::string& pattern) {
        b::log::internal::print_logger->set_pattern(pattern);
    }

}
