
#ifndef _BATTERY_PCH_H  // pragma in pch causes a warning on gcc
#define _BATTERY_PCH_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

//#define PRINT_PLATFORMS

// Platform & Compiler definitions
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
    #error [Battery] Unknown compiler architecture! None of the supported compilers have been detected! Edit include/Battery/pch.hpp to add it at your own risk
#endif

// Platform definitions
#if _WIN32 || _WIN64
    #define BATTERY_ARCH_WINDOWS     // Target platform is Windows
    #ifdef _WIN64
        #define BATTERY_ARCH_64
        #ifdef PRINT_PLATFORMS
            #pragma message "[Battery] Compiling for Windows/x64"
        #endif
    #else
        #define BATTERY_ARCH_32
        #ifdef PRINT_PLATFORMS
            #pragma message "[Battery] Compiling for Windows/x86"
        #endif
    #endif

#elif __linux__
    #define BATTERY_ARCH_LINUX       // Target platform is a variation of Linux

    // First check if it's ARM
    #if defined(__aarch64__) || defined(_M_ARM64)
        #define BATTERY_ARCH_64
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM64
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM 64"
        #endif
    #elif defined(__ARM_ARCH_2__) || defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__)
        #define BATTERY_ARCH_32
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM32
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM 32"
        #endif
    #elif defined(__x86_64__) || defined(__ppc64__) || defined(_M_X64)
        #define BATTERY_ARCH_64
        #define BATTERY_ARCH_AMD64
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/amd64 (64-bit)"
        #endif
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
        #define BATTERY_ARCH_86
        #define BATTERY_ARCH_i386
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/i386 (32-bit)"
        #endif
    #else
        #error [Battery] Unknown architecure: None of the supported architectures have been detected! Edit include/Battery/pch.hpp to add it at your own risk
    #endif

    /*#elif defined(__ARM_ARCH_2__)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM2
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM2"
        #endif
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM3
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM3"
        #endif
    #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM4T
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM4T"
        #endif
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM5
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM5"
        #endif
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM6T2
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM6T2"
        #endif
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM6
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM6"
        #endif
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM7
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM7"
        #endif
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define BATTERY_ARCH_ARM
        #define BATTERY_ARCH_ARM64
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/ARM64"
        #endif
    #elif defined(mips) || defined(__mips__) || defined(__mips)
        #define BATTERY_ARCH_MIPS
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/MIPS"
        #endif
    #elif defined(__sh__)
        #define BATTERY_ARCH_SH
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/SH"
        #endif
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        #define BATTERY_ARCH_POWERPC
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/POWERPC"
        #endif
    #elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
        #define BATTERY_ARCH_POWERPC64
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/POWERPC64"
        #endif
    #elif defined(__sparc__) || defined(__sparc)
        #define BATTERY_ARCH_SPARC
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/SPARC"
        #endif
    #elif defined(__m68k__)
        #define BATTERY_ARCH_M68K
        #ifdef PRINT_PLATFORMS
        #pragma message "[Battery] Compiling for Linux/M68K"
        #endif
    #else
        #pragma message "[Battery] Compiling for Linux/<unknown architecture>"
    #endif*/
#elif __sun
    #error [Battery] Solaris architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __FreeBSD__
    #error [Battery] FreeBSD architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __NetBSD__
    #error [Battery] NetBSD architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __OpenBSD__
    #error [Battery] OpenBSD architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __APPLE__
    #error [Battery] Mac OS X architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __hpux
    #error [Battery] HP-UX architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __osf__
    #error [Battery] TRU64 UNIX architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif __sgi
    #error [Battery] Irix architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#elif _AIX
    #error [Battery] AIX architecture is not supported yet! Feel free to add support yourself by editing include/Battery/pch.hpp, at your own risk.
#else
    #error [Battery] Unknown architecure! None of the supported architectures have been detected! Edit include/Battery/pch.hpp to add it at your own risk
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
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wvolatile"		// Disable a glm warning (because of C++20)
#endif

#include "glm/glm.hpp"

#ifdef BATTERY_COMPILER_GCC
//#pragma GCC diagnostic pop
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
