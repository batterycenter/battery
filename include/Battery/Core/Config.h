#pragma once

// Set build configurations
#if defined(DEBUG) && !defined(NDEBUG) && !defined(DEPLOY) && defined(NDEPLOY)
	#define BATTERY_DEBUG
#elif !defined(DEBUG) && defined(NDEBUG) && !defined(DEPLOY) && defined(NDEPLOY)
	#define BATTERY_RELEASE
#elif !defined(DEBUG) && defined(NDEBUG) && defined(DEPLOY) && !defined(NDEPLOY)
	#define BATTERY_DEPLOY
#else
	#error Invalid build configuration: A valid combination of the preprocessor macros 'DEBUG', 'NDEBUG', 'DEPLOY' and 'NDEPLOY' must be defined globally!
#endif

// Enable logging, uncomment to enable
#if defined(BATTERY_DEBUG)
	#define BATTERY_LOGGING
#elif defined(BATTERY_RELEASE)
	#define BATTERY_LOGGING
#elif defined(BATTERY_DEPLOY)
//	#define BATTERY_LOGGING 
#endif

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

// General settings
#define BATTERY_DEFAULT_TITLE		"BatteryEngine Application"
#define BATTERY_DEFAULT_FOLDER_NAME "BatteryApplication"

// Profiling											// TODO: Remove profiling
#define BATTERY_PROFILING_MAX_TIMEPOINT_NUMBER 64
#define BATTERY_PROFILING_MAX_SCOPED_NUMBER 64
#define BATTERY_PROFILING_TIMEPOINT_STRING_LENGTH 64
#define BATTERY_PROFILING_SCOPED_STRING_LENGTH 64

// Networking
#define BATTERY_DEFAULT_UDP_BUFFER_SIZE 256

// Graphics
#define BATTERY_MIN_WINDOW_WIDTH 200
#define BATTERY_MIN_WINDOW_HEIGHT 200
#define BATTERY_ANTIALIASING_LINE_FALLOFF 1
#define BATTERY_DEFAULT_BACKGROUND_COLOR glm::vec4(60, 60, 60, 255)

// File I/O
#define BATTERY_FILE_BLOCK_SIZE 1024

// Enable experimental allegro features
#define ALLEGRO_UNSTABLE
