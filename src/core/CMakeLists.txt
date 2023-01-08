
battery_add_package("gh:gabime/spdlog#v1.11.0")
battery_add_package("gh:ToruNiina/toml11#v3.7.1")

battery_add_library(battery_core STATIC                 # Also add all headers so they're shown correctly in IDEs

    include/battery/core/extern/CLI11.hpp
    include/battery/core/extern/magic_enum.hpp
    include/battery/core/extern/rang.hpp
    include/battery/core/extern/semver.hpp

    include/battery/core/all.h
    include/battery/core/common.h
    include/battery/core/environment.h
    include/battery/core/fs.h
    include/battery/core/log.h
    include/battery/core/main.h
    include/battery/core/OsString.h
    include/battery/core/platform.h
    include/battery/core/string.h

    src/fs.cpp
    src/log.cpp
    src/main.cpp
    src/OsString.cpp
    src/platform.cpp
    src/string.cpp

    )
add_library(battery::core ALIAS battery_core)

target_include_directories(battery_core PUBLIC include)

target_link_libraries(battery_core spdlog::spdlog toml11::toml11)