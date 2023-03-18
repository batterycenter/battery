
option(BATTERY_CORE_NO_MAIN "Prevent battery from defining the main() function internally" OFF)
option(BATTERY_CORE_NO_TRAY "Compile without support for system tray icons (without libappindicator dependency)" OFF)

# Battery core library
battery_add_library(battery_core STATIC ALIAS battery::core
        src/extern/platform_folders.cpp
        src/extern/utf8proc.c
        src/extern/utf8proc_data.c
        src/internal/platform.cpp
        src/tray/components/button.cpp
        src/tray/components/imagebutton.cpp
        src/tray/components/label.cpp
        src/tray/components/separator.cpp
        src/tray/components/submenu.cpp
        src/tray/components/syncedtoggle.cpp
        src/tray/components/toggle.cpp
        src/tray/core/linux/icon.cpp
        src/tray/core/linux/image.cpp
        src/tray/core/linux/tray.cpp
        src/tray/core/windows/icon.cpp
        src/tray/core/windows/image.cpp
        src/tray/core/windows/tray.cpp
        src/tray/core/entry.cpp
        src/tray/core/traybase.cpp
        src/constants.cpp
        src/fs.cpp
        src/log.cpp
        src/main.cpp
        src/messages.cpp
        src/platform.cpp
        src/process.cpp
        src/resource.cpp
        src/string.cpp
        src/time.cpp)

if (MSVC)               # For Visual Studio we add all header files as sources so they're listed in the IDE
    file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**)
    target_sources(battery_core PUBLIC ${HEADERS})
endif()

# Properties
target_include_directories(battery_core PUBLIC include)
target_compile_definitions(battery_core PUBLIC UTF8PROC_STATIC)

# The core::main module contains the main() function
add_subdirectory(main)
if (NOT BATTERY_CORE_NO_MAIN)
    target_link_libraries(battery_core battery::core::main)
endif()

# Tray module
if (UNIX AND NOT BATTERY_CORE_NO_TRAY)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)

    target_include_directories(battery_core SYSTEM PUBLIC ${GTK3_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})
    target_link_libraries(battery_core ${GTK3_LIBRARIES} ${APPINDICATOR_LIBRARIES})
endif()
if (BATTERY_CORE_NO_TRAY)
    target_compile_definitions(battery_core PUBLIC BATTERY_CORE_NO_TRAY)
endif()

# Download dependencies
set(REPROC++ ON)
battery_add_package("gh:DaanDeMeyer/reproc#v14.2.4")    # Library for running processes
battery_add_package("gh:gabime/spdlog#v1.11.0")         # Logging library
battery_add_package("gh:ToruNiina/toml11#v3.7.1")       # Toml fileformat parsing
battery_add_package("gh:nemtrif/utfcpp#v3.2.3")         # UTF-8 conversions

# Link dependencies
target_link_libraries(battery_core
        spdlog::spdlog
        toml11::toml11
        utf8::cpp
        reproc++
    )

# Unit-testing
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()