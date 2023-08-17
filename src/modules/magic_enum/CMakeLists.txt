
# Usage of this module:
# battery_add_module(magic_enum)
# target_link_libraries(... battery::magic_enum)

# Download the toml11 toml fileformat parsing library from Github
battery_add_package("gh:Neargye/magic_enum#v0.8.2")
b_add_alias_library(battery::magic_enum magic_enum)

b_set_target_ide_folder(magic_enum "battery/modules")