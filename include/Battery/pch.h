
#ifndef _BATTERY_PCH_H  // pragma causes a warning on gcc
#define _BATTERY_PCH_H

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

#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"		// Disable a glm warning (because of C++20)
#endif

	#include "glm/glm.hpp"                  

#ifndef _WIN32
#pragma GCC diagnostic pop
#endif

#include "nlohmann/json.hpp"

// ImGui library
#include "imgui.h"
#include "imgui_internal.h"
#include "../res/bindings/imgui_impl_allegro5.h"
#include "implot.h"

#endif // _BATTERY_PCH_H
