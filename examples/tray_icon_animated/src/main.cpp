
#include "battery/core/all.hpp"

#ifndef EXAMPLE_ROOT
#error EXAMPLE_ROOT is not defined, the build system is probably not set up correctly
#endif

int b::main(const std::vector<std::string>& args) {
    battery::log::info("Running battery example tray-icon-animated. Have fun!");

    // First load the frames into memory
    std::vector<b::resource> frames;
    for (int i = 1; i <= 32; i++) {
        b::fs::path path = EXAMPLE_ROOT;
        path += fmt::format("resources/fire{}.png", i);
        frames.emplace_back(b::resource::from_binary_file(path));
    }
    auto defaultIcon = b::resource::from_base64(b::constants::battery_icon_base64(), "png");

    // Everything related to one Tray class must be called from the same thread.
    // You can easily dispatch this to a background thread using battery::async_worker,
    // but the class instantiation and event loop must be in the same worker.

    bool animate = true;                           // Here, the MouseButton is which button opens the context menu (optional)
    b::tray::tray tray("MyTray", "Example Tray Icon Animated", b::tray::MouseButton::RIGHT);
    tray.addClickCallback(b::tray::MouseButton::LEFT, [] {
        battery::log::info("Left mouse button");
    });
    tray.addClickCallback(b::tray::MouseButton::RIGHT, [] {
        battery::log::info("Right mouse button");
    });

    // Here define the options for the context menu. You don't have to define anything.
    tray.addEntry(b::tray::button("Quit", [&tray] { tray.exit(); }));
    tray.addEntry(b::tray::button("Disabled button"))->setDisabled(true);
    tray.addEntry(b::tray::separator());
    tray.addEntry(b::tray::label("Just a label"));
    tray.addEntry(b::tray::toggle("Animate", true, [&animate](bool state) {
        if (state && !animate) {
            battery::log::info("Start animation");
            animate = true;
        }
        if (!state && animate) {
            battery::log::info("Stop animation");
            animate = false;
        }
    }));
    tray.addEntry(b::tray::separator());
    bool synced = false;
    auto sub = tray.addEntry(b::tray::submenu("Test Submenu"));
    sub->addEntry(b::tray::synced_toggle("Synced toggle 1", synced, [] (bool b) {
        battery::log::warn("Synced toggle 1: {}", b);
    }));
    sub->addEntry(b::tray::synced_toggle("Synced toggle 2", synced, [] (bool b) {
        battery::log::warn("Synced toggle 2: {}", b);
    }));

    // And start the loop :)
    int i = 0;
    while (tray.run_nonblocking()) {
        b::sleep_ms(30);
        tray.setIcon(animate ? frames[i++] : defaultIcon);
        i %= frames.size();
        //battery::log::warn("Synced: {}", synced);
    }

    return 0;
}
