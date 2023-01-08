
#ifndef _BATTERY_PCH_H  // pragma in pch causes a warning on gcc
#define _BATTERY_PCH_H

#include "common.h"

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

#endif // _BATTERY_PCH_H
