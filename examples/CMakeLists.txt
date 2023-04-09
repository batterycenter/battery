
add_subdirectory(BatteryApp)
add_subdirectory(BatteryEmbed)

if (NOT BATTERY_CORE_NO_TRAY)
    add_subdirectory(TrayIconAnimated)
endif ()