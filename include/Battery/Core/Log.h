#pragma once

#include "Battery/common.h"
#include "Battery/Core/Config.h"

namespace Battery::Log {

    extern std::shared_ptr<spdlog::logger> core_logger;
    extern std::shared_ptr<spdlog::logger> user_logger;

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
        if (!core_logger || !user_logger) {
            core_logger = spdlog::stdout_color_mt("battery");
            core_logger->set_level((spdlog::level::level_enum)Level::Info);
            core_logger->set_pattern("%^[%T] [%n] %v%$");
            user_logger = spdlog::stdout_color_mt("user");
            user_logger->set_level((spdlog::level::level_enum)Level::Info);
            user_logger->set_pattern("%^[%T] %v%$");
        }
    }

    inline void Level(enum Level level) {
        user_logger->set_level((spdlog::level::level_enum)level);
    }

    inline void Pattern(const std::string& pattern) {
        user_logger->set_pattern(pattern);
    }

    template<typename... T> auto Trace(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->trace(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Debug(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->debug(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Info(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->info(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Warn(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->warn(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Error(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->error(fmt, std::forward<T>(args)...); }
    template<typename... T> auto Critical(fmt::format_string<T...> fmt, T&&... args) { __init(); user_logger->critical(fmt, std::forward<T>(args)...); }




    namespace Core {

        inline void Level(enum Level level) {
            core_logger->set_level((spdlog::level::level_enum)level);
        }

        inline void Pattern(const std::string& pattern) {
            core_logger->set_pattern(pattern);
        }

        template<typename... T> auto Trace(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->trace(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Debug(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->debug(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Info(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->info(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Warn(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->warn(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Error(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->error(fmt, std::forward<T>(args)...); }
        template<typename... T> auto Critical(fmt::format_string<T...> fmt, T&&... args) { __init(); core_logger->critical(fmt, std::forward<T>(args)...); }

    }

}
