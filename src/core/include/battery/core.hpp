#ifndef BATTERY_CORE_HPP
#define BATTERY_CORE_HPP

//
// Copyright 2022 Florian Zachs (HerrNamenlos123)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "battery/core/async_worker.hpp"
#include "battery/core/base64.hpp"
#include "battery/core/cache.hpp"
#include "battery/core/color.hpp"
#include "battery/core/console.hpp"
#include "battery/core/console_application.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/constexpr.hpp"
#include "battery/core/containers.hpp"
#include "battery/core/error.hpp"
#include "battery/core/exception.hpp"
#include "battery/core/file_watcher.hpp"
#include "battery/core/filedialog.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/format.hpp"
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
#include "battery/core/tray.hpp"
#include "battery/core/uuid.hpp"
#include "battery/core/vec.hpp"

#include "battery/core/extern/CLI11.hpp"        // TODO: Think about moving these to different files
#include "battery/core/extern/indicators.hpp"   // TODO: Think about moving these to different files
#include "battery/core/extern/json.hpp"         // TODO: Think about moving these to different files

#endif // BATTERY_CORE_HPP