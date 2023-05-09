
battery_add_executable(Example_SteamLoginPage
        src/App.cpp
        src/main.cpp)

battery_add_module(graphics)
target_link_libraries(Example_SteamLoginPage battery::graphics)