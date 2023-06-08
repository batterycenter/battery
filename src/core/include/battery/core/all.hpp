#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <mutex>
#include <source_location>

#include "battery/core/async_worker.hpp"
#include "battery/core/cache.hpp"
#include "battery/core/console.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/error.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/log.hpp"
#include "battery/core/main.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/process.hpp"
#include "battery/core/resource.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/string.hpp"
#include "battery/core/thread.hpp"
#include "battery/core/time.hpp"
#include "battery/core/uuid.hpp"
#include "battery/core/resource_loader.hpp"

//#ifdef B_COMPILER_MSVC
//#pragma warning( push )
//#pragma warning( disable : 4244 )
//#endif

#include "battery/core/extern/CLI11.hpp"        // TODO: Think about moving these to different files
#include "battery/core/extern/indicators.hpp"   // TODO: Think about moving these to different files
#include "battery/core/extern/json.hpp"         // TODO: Think about moving these to different files

//#ifdef B_COMPILER_MSVC
//#pragma warning( pop )
//#endif
