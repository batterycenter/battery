
# Usage of this module:
# battery_add_module(spdlog)
# target_link_libraries(... battery::spdlog)

# Download the spdlog logging library from Github

battery_add_package("gh:gabime/spdlog#v1.11.0")
battery_add_alias_library(battery::spdlog spdlog)

battery_set_target_ide_folder(spdlog "battery/modules")