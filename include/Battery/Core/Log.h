#pragma once

// Do not include common.h here, instead this file is included in common.h!!!

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "Battery/Core/Config.h"

namespace Battery::Log {

    namespace Internal {
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
        if (!Internal::core_logger || !Internal::user_logger) {
            Internal::core_logger = spdlog::stdout_color_mt("battery");
            Internal::core_logger->set_level((spdlog::level::level_enum)Level::Info);
            Internal::core_logger->set_pattern("%^[%T] [%n] %v%$");
            Internal::user_logger = spdlog::stdout_color_mt("user");
            Internal::user_logger->set_level((spdlog::level::level_enum)Level::Info);
            Internal::user_logger->set_pattern("%^[%T] %v%$");
        }
    }

    inline void Level(enum Level level) {
        Internal::user_logger->set_level((spdlog::level::level_enum)level);
    }

    inline void Pattern(const std::string& pattern) {
        Internal::user_logger->set_pattern(pattern);
    }

    template<typename... T> auto Trace(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->trace(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Debug(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->debug(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Info(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->info(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Warn(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->warn(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Error(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->error(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Critical(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::user_logger->critical(fmt, std::forward<T>(args)...); }




    namespace Core {

        inline void Level(enum Level level) {
            Internal::core_logger->set_level((spdlog::level::level_enum)level);
        }

        inline void Pattern(const std::string& pattern) {
            Internal::core_logger->set_pattern(pattern);
        }

        template<typename... T> auto Trace(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->trace(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Debug(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->debug(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Info(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->info(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Warn(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->warn(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Error(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->error(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Critical(fmt::format_string<T...> fmt, T&&... args) { __init(); Internal::core_logger->critical(fmt, std::forward<T>(args)...); }

    }

}
