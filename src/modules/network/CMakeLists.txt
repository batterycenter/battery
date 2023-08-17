
# Usage of this module:
# battery_add_module(network)
# target_link_libraries(... PUBLIC battery::network)

b_add_library(battery_network STATIC ALIAS battery::network)
b_add_source_files(battery_network
        src/network.cpp
        )

b_set_ide_folder(battery_network "battery/modules")

battery_add_module(sfml)
target_link_libraries(battery_network PUBLIC 
        battery::core
        battery::sfml::network
    )
