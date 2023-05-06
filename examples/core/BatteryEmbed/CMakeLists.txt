
battery_add_executable(Example_BatteryEmbed src/main.cpp)
target_link_libraries(Example_BatteryEmbed battery::core)

battery_embed(Example_BatteryEmbed "${CMAKE_CURRENT_LIST_DIR}/resources/message.txt" TEXT)
battery_embed(Example_BatteryEmbed "${CMAKE_CURRENT_LIST_DIR}/resources/banner.txt" TEXT)
