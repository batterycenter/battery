
# Usage of this module:
# battery_add_module(tray)
# target_link_libraries(... battery::tray)

# Battery core library
battery_add_library(battery_tray STATIC ALIAS battery::tray)
battery_add_source_files(battery_tray   # This function also groups the files in the IDE
        src/components/button.cpp
        src/components/label.cpp
        src/components/separator.cpp
        src/components/submenu.cpp
        src/components/syncedtoggle.cpp
        src/components/toggle.cpp
        src/core/linux/tray.cpp
        src/core/windows/tray.cpp
        src/core/entry.cpp
        src/core/traybase.cpp
    )

target_link_libraries(battery_tray battery::core)

if (UNIX)
    find_package(PkgConfig)
    if (NOT PKG_CONFIG_FOUND)
        message(FATAL_ERROR "PkgConfig not found! Please install them using your package manager \n(Hint: 'sudo apt install pkg-config'), or use -DBATTERY_CORE_NO_TRAY=true to compile without tray icon support.")
    endif()
    pkg_check_modules(GTK3 gtk+-3.0)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR "GTK3 development libraries not found! Please install them using your package manager \n(Hint: 'sudo apt install libgtk-3-dev'), or use -DBATTERY_CORE_NO_TRAY=true to compile without tray icon support.")
    endif()
    pkg_check_modules(LIBAPPINDICATOR ayatana-appindicator3-0.1)
    if (NOT LIBAPPINDICATOR_FOUND)
        message(FATAL_ERROR "Appindicator development libraries not found! \nEither install them using your package manager \n(Hint: 'sudo apt install libayatana-appindicator3-dev'), or use -DBATTERY_CORE_NO_TRAY=true to compile without tray icon support.")
    endif()
    target_include_directories(battery_tray PRIVATE ${LIBAPPINDICATOR_INCLUDE_DIRS})
    target_link_libraries(battery_tray ${LIBAPPINDICATOR_LIBRARIES})
endif()