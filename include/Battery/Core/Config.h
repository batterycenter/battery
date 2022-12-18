#pragma once

#include <string>

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
