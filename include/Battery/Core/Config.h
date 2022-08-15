#pragma once

#include <string>

// Log levels
#define BATTERY_LOG_LEVEL_CRITICAL	spdlog::level::critical
#define BATTERY_LOG_LEVEL_ERROR		spdlog::level::err
#define BATTERY_LOG_LEVEL_WARN		spdlog::level::warn
#define BATTERY_LOG_LEVEL_INFO		spdlog::level::info
#define BATTERY_LOG_LEVEL_DEBUG		spdlog::level::debug
#define BATTERY_LOG_LEVEL_TRACE		spdlog::level::trace

#define BATTERY_DEFAULT_LOG_LEVEL	BATTERY_LOG_LEVEL_DEBUG	
#define BATTERY_CORE_LOGGER_NAME	"BatteryCore"				// Display names for console logging
#define BATTERY_CLIENT_LOGGER_NAME	"Application"

#define DEFAULT_FONT_SIZE 16

// General settings
#define BATTERY_DEFAULT_TITLE				"Battery Application"
#define BATTERY_DEFAULT_APPLICATION_NAME	"BatteryApplication"

// Profiling											// TODO: Remove profiling
#define BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER 64
#define BATTERY_PROFILING_MAX_SCOPED_NUMBER 64
#define BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH 64
#define BATTERY_PROFILING_SCOPED_STRING_LENGTH 64

// Networking
#define BATTERY_DEFAULT_UDP_BUFFER_SIZE 256

// Graphics
#define BATTERY_ANTIALIASING_LINE_FALLOFF 1
#define BATTERY_DEFAULT_BACKGROUND_COLOR { 60, 60, 60, 255 }

extern const char* BATTERY_DEFAULT_WINDOW_ICON_BASE64;
extern const size_t BATTERY_DEFAULT_WINDOW_ICON_SIZE;
