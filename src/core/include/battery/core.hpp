#pragma once

#include "battery/core/async_worker.hpp"
#include "battery/core/cache.hpp"
#include "battery/core/console.hpp"
#include "battery/core/console_application.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/containers.hpp"
#include "battery/core/error.hpp"
#include "battery/core/expected.hpp"
#include "battery/core/file_watcher.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/inja.hpp"
#include "battery/core/lockfile.hpp"
#include "battery/core/log.hpp"
#include "battery/core/magic_enum.hpp"
#include "battery/core/main.hpp"
#include "battery/core/map.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/process.hpp"
#include "battery/core/random.hpp"
#include "battery/core/resource.hpp"
#include "battery/core/resource_loader.hpp"
#include "battery/core/semver.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/string.hpp"
#include "battery/core/thread.hpp"
#include "battery/core/time.hpp"
#include "battery/core/uuid.hpp"

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
