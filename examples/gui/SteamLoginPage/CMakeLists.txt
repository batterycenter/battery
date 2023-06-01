
battery_add_executable(Example_SteamLoginPage
        src/App.cpp
        src/main.cpp)

battery_add_module(graphics)
battery_add_module(qrcode)
target_link_libraries(Example_SteamLoginPage battery::graphics battery::qrcode)

battery_precompile_headers(Example_SteamLoginPage "${CMAKE_CURRENT_LIST_DIR}/include/pch.hpp")

battery_embed(Example_SteamLoginPage ${CMAKE_CURRENT_LIST_DIR}/ui/main.py ui_main_py TEXT)