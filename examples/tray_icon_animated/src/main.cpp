
#include "battery/core/all.h"

#ifndef EXAMPLE_ROOT
#error EXAMPLE_ROOT is not defined, the build system is probably not set up correctly
#endif

int battery_main(const std::vector<std::string>& args) {
    battery::log::info("Running battery example tray-icon-animated. Have fun!");

    // First load the frames into memory
    std::vector<battery::resource> frames;
    for (int i = 1; i <= 32; i++) {
        battery::fs::path path = EXAMPLE_ROOT;
        path += fmt::format("resources/fire{}.png", i);
        frames.emplace_back(battery::resource::from_binary_file(path));
    }

    // Create the tray icon
    auto& tray = battery::tray_icon::get();
    tray.create();

    // And start the loop :)
    while (battery::time() < 30) {
        for (const auto& frame : frames) {
            tray.set_icon(frame);
            battery::sleep(0.02);
        }
    }

    tray.destroy();     // Not actually necessary
    return 0;
}
