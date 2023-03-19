#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <mutex>
#include <source_location>

#include "battery/core/async_worker.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/environment.hpp"
#include "battery/core/error.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/log.hpp"
#include "battery/core/main.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/platform.hpp"
#include "battery/core/process.hpp"
#include "battery/core/resource.hpp"
#include "battery/core/string.hpp"
#include "battery/core/thread.hpp"
#include "battery/core/time.hpp"
#include "battery/core/tray/tray.hpp"

#include "toml.hpp"

#ifdef BATTERY_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

#include "battery/core/extern/CLI11.hpp"
#include "battery/core/extern/inja.hpp"
#include "battery/core/extern/json.hpp"
#include "battery/core/extern/magic_enum.hpp"
#include "battery/core/extern/rang.hpp"
#include "battery/core/extern/semver.hpp"

#ifdef BATTERY_COMPILER_MSVC
#pragma warning( pop )
#endif
