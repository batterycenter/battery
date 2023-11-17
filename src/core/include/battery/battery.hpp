#ifndef BATTERY_BATTERY_HPP
#define BATTERY_BATTERY_HPP

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

#include "async_worker.hpp"
#include "base64.hpp"
#include "bytearray.hpp"
#include "cache.hpp"
#include "color.hpp"
#include "console.hpp"
#include "application.hpp"
#include "constants.hpp"
#include "constexpr.hpp"
#include "containers.hpp"
#include "error.hpp"
#include "exception.hpp"
#include "file_watcher.hpp"
#include "filedialog.hpp"
#include "folders.hpp"
#include "format.hpp"
#include "fs.hpp"
#include "inja.hpp"
#include "lockfile.hpp"
#include "log.hpp"
#include "magic_enum.hpp"
#include "map.hpp"
#include "messages.hpp"
#include "process.hpp"
#include "random.hpp"
#include "resource.hpp"
#include "resource_loader.hpp"
#include "semver.hpp"
#include "signal.hpp"
#include "string.hpp"
#include "thread.hpp"
#include "time.hpp"
#include "tray.hpp"
#include "uuid.hpp"
#include "vec.hpp"
#include "window.hpp"

#include "battery/cli11.hpp"

#include "battery/extern/indicators.hpp"   // TODO: Think about moving these to different files
#include "battery/extern/json.hpp"         // TODO: Think about moving these to different files

#endif // BATTERY_BATTERY_HPP