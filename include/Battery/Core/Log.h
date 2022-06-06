#pragma once

#ifndef SPDLOG		// This prevents the header from being included if there's 
#define SPDLOG		// already a logging library available from another library

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "Battery/Core/Config.h"

namespace Battery {
	namespace Log {

		extern std::shared_ptr<spdlog::logger> coreLogger;
		extern std::shared_ptr<spdlog::logger> clientLogger;

	}
}

// TODO: Make logging more fool-proof (and check if name already exists, prevent crashes)

#define LOG_SET_BATTERY_LOGLEVEL(...)	Battery::Log::coreLogger->set_level(__VA_ARGS__)
#define LOG_SET_CLIENT_LOGLEVEL(...)	Battery::Log::clientLogger->set_level(__VA_ARGS__)
#define LOG_SET_LOGLEVEL(...)			{ LOG_SET_BATTERY_LOGLEVEL(__VA_ARGS__); LOG_SET_CLIENT_LOGLEVEL(__VA_ARGS__); }

#define BATTERY_INIT_LOGGER()			{	if (!Battery::Log::coreLogger || !Battery::Log::clientLogger) {	\
												spdlog::set_pattern("%^[%T] %n: %v%$"); \
												Battery::Log::coreLogger = spdlog::stdout_color_mt(BATTERY_CORE_LOGGER_NAME); \
												Battery::Log::clientLogger = spdlog::stdout_color_mt(BATTERY_CLIENT_LOGGER_NAME); \
												LOG_SET_LOGLEVEL(BATTERY_DEFAULT_LOG_LEVEL); \
											} \
										}

#define LOG_CORE_TRACE(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->trace(__VA_ARGS__);				 }
#define LOG_CORE_WARN(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->warn(__VA_ARGS__);				 }
#define LOG_CORE_DEBUG(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->debug(__VA_ARGS__);				 }
#define LOG_CORE_INFO(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->info(__VA_ARGS__);				 }
#define LOG_CORE_ERROR(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->error(__VA_ARGS__);				 }
#define LOG_CORE_CRITICAL(...)			{ BATTERY_INIT_LOGGER(); Battery::Log::coreLogger->critical(__VA_ARGS__);			 }
#define LOG_TRACE(...)					{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->trace(__VA_ARGS__);			 }
#define LOG_WARN(...)					{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->warn(__VA_ARGS__);				 }
#define LOG_DEBUG(...)					{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->debug(__VA_ARGS__);			 }
#define LOG_INFO(...)					{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->info(__VA_ARGS__);				 }
#define LOG_ERROR(...)					{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->error(__VA_ARGS__);			 }
#define LOG_CRITICAL(...)				{ BATTERY_INIT_LOGGER(); Battery::Log::clientLogger->critical(__VA_ARGS__);			 }

#else

#define LOG_SET_BATTERY_LOGLEVEL(...)	{ ; }
#define LOG_SET_CLIENT_LOGLEVEL(...)	{ ; }
#define LOG_SET_LOGLEVEL(...)			{ ; }

#define LOG_CORE_TRACE(...)				{ ; }
#define LOG_CORE_WARN(...)				{ ; }
#define LOG_CORE_DEBUG(...)				{ ; }
#define LOG_CORE_INFO(...)				{ ; }
#define LOG_CORE_ERROR(...)				{ ; }
#define LOG_CORE_CRITICAL(...)			{ ; }
#define LOG_TRACE(...)					{ ; }
#define LOG_WARN(...)					{ ; }
#define LOG_DEBUG(...)					{ ; }
#define LOG_INFO(...)					{ ; }
#define LOG_ERROR(...)					{ ; }
#define LOG_CRITICAL(...)				{ ; }

#endif // SPDLOG
