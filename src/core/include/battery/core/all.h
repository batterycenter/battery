#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <mutex>
#include <source_location>

#include "battery/core/async_worker.h"
#include "battery/core/constants.h"
#include "battery/core/environment.h"
#include "battery/core/error.h"
#include "battery/core/fs.h"
#include "battery/core/log.h"
#include "battery/core/main.h"
#include "battery/core/messages.h"
#include "battery/core/platform.h"
#include "battery/core/process.h"
#include "battery/core/resource.h"
#include "battery/core/string.h"
#include "battery/core/thread.h"
#include "battery/core/time.h"
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
//#include "battery/core/extern/platform_folders.h"
#include "battery/core/extern/rang.hpp"
#include "battery/core/extern/semver.hpp"
//#include "battery/core/extern/tray.h"

#ifdef BATTERY_COMPILER_MSVC
#pragma warning( pop )
#endif
