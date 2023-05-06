
# Usage of this module:
# battery_add_module(sfml)
#  - target_link_libraries(<target>
#        battery::sfml::graphics
#        battery::sfml::window
#        battery::sfml::system
#        battery::sfml::audio
#        battery::sfml::network
#    )

# Download the SFML Framework from Github: Latest commit from Apr 25, 2023
battery_add_package("gh:SFML/SFML#cbfa9cbb65c0974ade5c52b182e8c57975d4d693")

battery_add_alias_library(battery::sfml::graphics sfml-graphics)
battery_add_alias_library(battery::sfml::window sfml-window)
battery_add_alias_library(battery::sfml::system sfml-system)
battery_add_alias_library(battery::sfml::audio sfml-audio)
battery_add_alias_library(battery::sfml::network sfml-network)

battery_set_target_ide_folder(sfml-audio "battery/modules")
battery_set_target_ide_folder(sfml-graphics "battery/modules")
battery_set_target_ide_folder(sfml-network "battery/modules")
battery_set_target_ide_folder(sfml-system "battery/modules")
battery_set_target_ide_folder(sfml-window "battery/modules")