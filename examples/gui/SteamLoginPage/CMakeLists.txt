battery_add_example(Example_SteamLoginPage
        src/App.cpp
        src/main.cpp)

battery_add_module(graphics)
battery_add_module(qrcode)
target_link_libraries(Example_SteamLoginPage battery::graphics battery::qrcode)

b_precompile_headers(Example_SteamLoginPage include/pch.hpp)
battery_embed(Example_SteamLoginPage ui/main.py ui_main_py TEXT)