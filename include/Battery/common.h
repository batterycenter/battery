#pragma once

#include "Environment.h"

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
#include <filesystem>

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"		// Disable a glm warning (C++20 and up)
#endif

#include "glm/glm.hpp"

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

#include "Battery/Extern/json.hpp"
#include "Battery/Extern/CLI11.hpp"

// ImGui library
#ifdef BATTERY_FEATURES_GRAPHICS
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
#endif

#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB     // Force SPDLOG to recognize the prebuilt library
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
