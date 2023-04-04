
option(BATTERY_CORE_NO_MAIN "Prevent battery from defining the main() function internally" OFF)
option(BATTERY_CORE_NO_TRAY "Compile without support for system tray icons (without libappindicator dependency)" OFF)

# Battery core library
battery_add_library(battery_core STATIC ALIAS battery::core)

set(SOURCES
        src/extern/platform_folders.cpp
        src/extern/utf8proc.c
        src/extern/utf8proc_data.c
        src/internal/folders.cpp

        src/tray/components/button.cpp
        src/tray/components/label.cpp
        src/tray/components/separator.cpp
        src/tray/components/submenu.cpp
        src/tray/components/syncedtoggle.cpp
        src/tray/components/toggle.cpp
        src/tray/core/linux/tray.cpp
        src/tray/core/windows/tray.cpp
        src/tray/core/entry.cpp
        src/tray/core/traybase.cpp

        src/cache.cpp
        src/console.cpp
        src/constants.cpp
        src/fs.cpp
        src/log.cpp
        src/main.cpp
        src/messages.cpp
        src/folders.cpp
        src/process.cpp
        src/resource.cpp
        src/signal.cpp
        src/string.cpp
        src/time.cpp
        src/uuid.cpp)

target_sources(battery_core PRIVATE ${SOURCES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})

if (MSVC)               # For Visual Studio we add all header files as sources so they're listed in the IDE
    file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**)
    target_sources(battery_core PRIVATE ${HEADERS})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
endif()

# Properties
target_include_directories(battery_core PUBLIC include)
target_compile_definitions(battery_core PUBLIC UTF8PROC_STATIC)

if (BATTERY_CORE_NO_MAIN)
    target_compile_definitions(battery_core BATTERY_CORE_NO_MAIN)
endif()

# Download dependencies
set(REPROC++ ON)
battery_add_package("gh:DaanDeMeyer/reproc#v14.2.4")            # Library for running processes
battery_add_package("gh:gabime/spdlog#v1.11.0")                 # Logging library
battery_add_package("gh:ToruNiina/toml11#v3.7.1")               # Toml fileformat parsing
battery_add_package("gh:nemtrif/utfcpp#v3.2.3")                 # UTF-8 conversions

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

# Tray icon dependencies
if (UNIX AND NOT BATTERY_CORE_NO_TRAY)
    find_package(PkgConfig)
    if (NOT PKG_CONFIG_FOUND)
        message(FATAL_ERROR "PkgConfig not found! Please install them using your package manager \n(Hint: 'sudo apt install pkg-config')")
    endif()
    pkg_check_modules(GTK3 gtk+-3.0)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR "GTK3 development libraries not found! Please install them using your package manager \n(Hint: 'sudo apt install libgtk-3-dev')")
    endif()
    pkg_check_modules(LIBAPPINDICATOR ayatana-appindicator3-0.1)
    if (NOT LIBAPPINDICATOR_FOUND)
        message(FATAL_ERROR "Appindicator development libraries not found! \nEither install them using your package manager \n(Hint: 'sudo apt install libayatana-appindicator3-dev'), or use -DBATTERY_CORE_NO_TRAY to compile without tray icon support.")
    endif()
    target_include_directories(battery_core PRIVATE ${LIBAPPINDICATOR_INCLUDE_DIRS})
    target_link_libraries(battery_core ${LIBAPPINDICATOR_LIBRARIES})
endif()
if (BATTERY_CORE_NO_TRAY)
    target_compile_definitions(battery_core PUBLIC BATTERY_CORE_NO_TRAY)
endif()

# ncurses dependencies
if (UNIX)
    find_package(Curses)
    if (NOT CURSES_FOUND)
        message(FATAL_ERROR "Curses development libraries not found! \nPlease install them using your package manager \n(Hint: 'sudo apt install libncurses-dev')")
    endif()
    target_include_directories(battery_core PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(battery_core ${CURSES_LIBRARIES})
endif()

set_target_properties(battery_core PROPERTIES FOLDER "battery")
set_target_properties(reproc PROPERTIES FOLDER "battery/deps")
set_target_properties(spdlog PROPERTIES FOLDER "battery/deps")
set_target_properties(reproc++ PROPERTIES FOLDER "battery/deps")
