#pragma once

// Do not include common.h here, instead this file is included in common.h!!!

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace battery::log {

    namespace internal {
        extern std::shared_ptr<spdlog::logger> core_logger;
        extern std::shared_ptr<spdlog::logger> user_logger;
    }

    enum class Level {
        Trace = SPDLOG_LEVEL_TRACE,
        Debug = SPDLOG_LEVEL_DEBUG,
        Info = SPDLOG_LEVEL_INFO,
        Warn = SPDLOG_LEVEL_WARN,
        Err = SPDLOG_LEVEL_ERROR,
        Critical = SPDLOG_LEVEL_CRITICAL,
        Off = SPDLOG_LEVEL_OFF
    };

    inline void __init() {
        if (!internal::core_logger || !internal::user_logger) {
            internal::core_logger = spdlog::stdout_color_mt("battery");
            internal::core_logger->set_level((spdlog::level::level_enum)Level::Debug);
            internal::core_logger->set_pattern("%^[%T] [%n] %v%$");
            internal::user_logger = spdlog::stdout_color_mt("user");
            internal::user_logger->set_level((spdlog::level::level_enum)Level::Debug);
            internal::user_logger->set_pattern("%^[%T] %v%$");
        }
    }

    inline void level(enum Level level) {
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
