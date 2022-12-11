
#ifndef _BATTERY_PCH_H  // pragma in pch causes a warning on gcc
#define _BATTERY_PCH_H

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
