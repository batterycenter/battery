#pragma once

#include "Battery/Core/Main.h"
#include "Battery/Core/log.h"
#include "Battery/Core/exception.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Utils/MathUtils.h"
#include "Battery/Utils/StringUtils.h"
#include "Battery/Utils/OsString.h"

#ifdef BATTERY_FEATURES_GRAPHICS
    #include "Battery/Core/Application.h"
    #include "Battery/Core/ImGuiPanel.h"
    #include "Battery/Utils/TimeUtils.h"
    #include "Battery/Utils/ImGuiUtils.h"
    #include "Battery/Utils/AppUtils.h"
    #include "BatteryUI/BatteryUI.h"
#endif

#include "Battery/Extern/json.hpp"
#include "Battery/Extern/CLI11.hpp"
#include "Battery/Extern/magic_enum.hpp"
#include "Battery/Extern/rang.hpp"
#include "Battery/Extern/semver.hpp"

#include "toml.hpp"
