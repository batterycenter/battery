#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <mutex>
#include <source_location>

#include "battery/core/async_worker.hpp"
#include "battery/core/cache.hpp"
#include "battery/core/console.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/environment.hpp"
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
#include "battery/core/tray/tray.hpp"

#include "toml.hpp"

#ifdef BATTERY_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

#include "battery/core/extern/CLI11.hpp"
#include "battery/core/extern/expected.hpp"
#include "battery/core/extern/indicators.hpp"
#include "battery/core/extern/inja.hpp"
#include "battery/core/extern/json.hpp"
#include "battery/core/extern/magic_enum.hpp"
#include "battery/core/extern/rang.hpp"
#include "battery/core/extern/semver.hpp"

namespace b {
    using namespace tl;     // So we can use b::expected<> instead of tl::expected<>
    namespace toml {
        using namespace ::toml;     // So we can write b::toml::something instead of just toml::something
    }
    namespace semver {
        using namespace ::semver;     // So we can write b::semver::something instead of just semver::something
    }
    namespace inja {
        using namespace ::inja;      // So we can write b::inja::something instead of just inja::something
    }
}

#ifdef BATTERY_COMPILER_MSVC
#pragma warning( pop )
#endif
