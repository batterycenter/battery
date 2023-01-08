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
#include <expected>

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"		// Disable a GCC-specific glm warning (C++20 and up)
#endif

#include "glm/glm.hpp"

#ifdef BATTERY_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

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

#include "Battery/Core/log.h"       // Almost everything needs logging
