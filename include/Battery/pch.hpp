
#ifndef _BATTERY_PCH_H  // pragma causes a warning on gcc
#define _BATTERY_PCH_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define PRINT_PLATFORMS

// Platform definitions
#if _WIN32 || _WIN64
    #define BATTERY_ARCH_WINDOWS     // Target platform is Windows
    #ifdef _WIN64
        #define BATTERY_ARCH_64BIT
        #ifdef PRINT_PLATFORMS
            #pragma message "[Battery] Compiling for Windows 64-bit"
        #endif
    #else
        #define BATTERY_ARCH_32BIT
        #ifdef PRINT_PLATFORMS
            #pragma message "[Battery] Compiling for Windows 32-bit"
        #endif
    #endif
#else
#error [Battery] Unknown architecure! None of the supported architectures have been detected!
#endif

// Compiler definitions
#if defined(__clang__)
    #define BATTERY_COMPILER_CLANG
    #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Detected clang compiler"
    #endif
#elif defined(__MINGW32__)
    #define BATTERY_COMPILER_MINGW
    #define BATTERY_COMPILER_GCC
    #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Detected MINGW GCC compiler"
    #endif
#elif defined(__GNUC__) || defined(__GNUG__)
    #define BATTERY_COMPILER_GCC
    #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Detected GCC compiler"
    #endif
#elif defined(_MSC_VER)
    #define BATTERY_COMPILER_MSVC
    #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Detected MSVC compiler"
    #endif
#else
    #pragma message "[Battery] No supported compiler detected. Your build might not work as expected!" // TODO: Check this
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <optional>
#include <iomanip>
#include <map>
#include <cstddef>
#include <thread>

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"		// Disable a glm warning (because of C++20)
#endif

	#include "glm/glm.hpp"

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

#include "Battery/Extern/json.hpp"

// ImGui library
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"

#include "BatteryUI/BatteryUI.h"

// SFML
#include "SFML/Main.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"

#include "imgui-SFML.h"

#endif // _BATTERY_PCH_H
