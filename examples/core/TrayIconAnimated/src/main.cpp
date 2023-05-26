
#include "battery/core/all.hpp"
#include "battery/tray/tray.hpp"

#ifndef EXAMPLE_ROOT
#error EXAMPLE_ROOT is not defined, the build system is probably not set up correctly
#endif

int b::main(const std::vector<b::string>& args) {
    b::log::info("Running battery example tray-icon-animated. Have fun!");

    // First load the frames into memory
    std::vector<b::resource> frames;
    for (int i = 1; i <= 32; i++) {
        b::fs::path path = EXAMPLE_ROOT;
        path += b::format("resources/fire{}.png", i);
        frames.emplace_back(b::resource::from_binary_file(path));
    }
    auto defaultIcon = b::resource::from_base64(b::constants::battery_icon_base64(), u8"png");

    // Everything related to one Tray class must be called from the same thread.
    // You can easily dispatch this to a background thread using battery::async_worker,
    // but the class instantiation and event loop must be in the same worker.

    bool animate = false;                           // Here, the MouseButton is which button opens the context menu (optional)
    b::tray::tray tray(u8"MyTray", u8"Example Tray Icon Animated", b::tray::MouseButton::RIGHT);
    tray.addClickCallback(b::tray::MouseButton::LEFT, [] {
        b::log::info("Left mouse button");
    });
    tray.addClickCallback(b::tray::MouseButton::RIGHT, [] {
        b::log::info("Right mouse button");
    });

    // Here define the options for the context menu. You don't have to define anything.
    tray.addEntry(b::tray::button(u8"Quit", [&tray] { tray.exit(); }));
    tray.addEntry(b::tray::button(u8"Disabled button"))->setDisabled(true);
    tray.addEntry(b::tray::separator());
    tray.addEntry(b::tray::label(u8"Just a label"));
    tray.addEntry(b::tray::toggle(u8"Animate", false, [&animate,&tray,&defaultIcon](bool state) {
        if (state && !animate) {
            b::log::info("Start animation");
            animate = true;
        }
        if (!state && animate) {
            b::log::info("Stop animation");
            tray.setIcon(defaultIcon);
            animate = false;
        }
    }));
    tray.addEntry(b::tray::separator());
    bool synced = false;
    auto sub = tray.addEntry(b::tray::submenu(u8"Test Submenu"));
    sub->addEntry(b::tray::synced_toggle(u8"Synced toggle 1", synced, [] (bool b) {
        b::log::warn("Synced toggle 1: {}", b);
    }));
    sub->addEntry(b::tray::synced_toggle(u8"Synced toggle 2", synced, [] (bool b) {
        b::log::warn("Synced toggle 2: {}", b);
    }));

    // And start the loop :)
    int i = 0;
    while (tray.run_nonblocking()) {
        b::sleep_ms(30);
        if (animate) {
            tray.setIcon(frames[i++]);
        }
        i %= frames.size();
    }

    return 0;
}
