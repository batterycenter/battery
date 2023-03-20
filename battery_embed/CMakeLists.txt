
battery_add_executable(battery_embed ALIAS battery::embed
        battery_embed.cpp
        )
target_link_libraries(battery_embed battery::core)
set_target_properties(battery_embed PROPERTIES FOLDER "battery")

set_target_properties(battery_embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/battery_embed_bin")
set_target_properties(battery_embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/battery_embed_bin")
set_target_properties(battery_embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/battery_embed_bin")
set_target_properties(battery_embed PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/battery_embed_bin")