
option(BATTERY_CORE_NO_MAIN "Prevent battery from defining the main() function internally" OFF)

# Battery core library
battery_add_library(battery_core STATIC ALIAS battery::core   # Also add all headers so they're shown correctly in IDEs
    include/battery/core/extern/CLI11.hpp
    include/battery/core/extern/inja.hpp
    include/battery/core/extern/json.hpp
    include/battery/core/extern/magic_enum.hpp
    include/battery/core/extern/rang.hpp
    include/battery/core/extern/semver.hpp

    include/battery/core/all.h
    include/battery/core/Application.h
    include/battery/core/detachable_thread.h
    include/battery/core/environment.h
    include/battery/core/error.h
    include/battery/core/fs.h
    include/battery/core/log.h
    include/battery/core/main.h
    include/battery/core/messages.h
    include/battery/core/OsString.h
    include/battery/core/platform.h
    include/battery/core/process.h
    include/battery/core/string.h
    include/battery/core/thread.h
    include/battery/core/time.h

    include/battery/core/internal/windows.h

    src/Application.cpp
    src/exception.cpp
    src/fs.cpp
    src/log.cpp
    src/main.cpp
    src/messages.cpp
    src/OsString.cpp
    src/platform.cpp
    src/process.cpp
    src/string.cpp
    src/time.cpp)

# Properties
target_include_directories(battery_core PUBLIC include)

if (BATTERY_CORE_NO_MAIN)
    target_compile_definitions(battery_core PUBLIC BATTERY_CORE_NO_MAIN)
endif()

# Download dependencies
set(REPROC++ ON)
battery_add_package("gh:DaanDeMeyer/reproc#v14.2.4")    # Library for running processes
battery_add_package("gh:gabime/spdlog#v1.11.0")         # Logging library
battery_add_package("gh:ToruNiina/toml11#v3.7.1")       # Toml fileformat parsing
battery_add_package("gh:nemtrif/utfcpp#v3.2.3")         # UTF-8 conversions

# Link dependencies
target_link_libraries(battery_core
    PUBLIC
        spdlog::spdlog
        toml11::toml11
        utf8::cpp
        reproc++
    PRIVATE
    )

# Unit-testing
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()