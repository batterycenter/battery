
# Usage of this module:
# battery_add_module(check)
# target_link_libraries(... battery::check)

# Pull latest commit from GitHub from Jan 4, 2016
battery_add_package("gh:mapbox/cncc#5fe381fe1d2810bb6ba8770c74d1551dac69a1c3")

# Target to execute the cncc script when building
add_custom_target(battery_check
    COMMAND cncc ${BATTERY_ROOT_DIR}/check.style --dbdir ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running cncc"
)

battery_set_target_ide_folder(battery_check "battery/modules")