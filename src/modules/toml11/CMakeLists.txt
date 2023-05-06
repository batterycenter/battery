
# Usage of this module:
# battery_add_module(toml11)
# target_link_libraries(... battery::toml11)

# Download the toml11 toml fileformat parsing library from Github
battery_add_package("gh:ToruNiina/toml11#v3.7.1")
battery_add_alias_library(battery::toml11 toml11)

battery_set_target_ide_folder(toml11 "battery/modules")