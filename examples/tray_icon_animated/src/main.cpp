
#include "battery/core/all.h"

#ifndef EXAMPLE_ROOT
#error EXAMPLE_ROOT is not defined, the build system is probably not set up correctly
#endif

class S {
public:
    S() {}

    void run() {
        thread = b::thread([this]() {
            th();
        });
    }

    void th() {
        data = "SAKDJ";
        std::cout << data;
    }

    std::string data;
    b::thread thread;
};

void foo() {
    S s;
    s.run();
};

int battery_main(const std::vector<std::string>& args) {
    battery::log::info("Running battery example tray-icon-animated. Have fun!");

    foo();

    // First load the frames into memory
    std::vector<battery::resource> frames;
    for (int i = 1; i <= 32; i++) {
        battery::fs::path path = EXAMPLE_ROOT;
        path += fmt::format("resources/fire{}.png", i);
        frames.emplace_back(battery::resource::from_binary_file(path));
    }
    auto defaultIcon = battery::resource::from_base64(battery::constants::BATTERY_DEFAULT_WINDOW_ICON_BASE64, "png");

    // Everything related to one Tray class must be called from the same thread.
    // You can easily dispatch this to a background thread using battery::async_worker,
    // but the class instantiation and event loop must be in the same worker.

    bool animate = true;                           // Here, the MouseButton is which button opens the context menu (optional)
    Tray::Tray tray("MyTray", "Example Tray Icon Animated", Tray::MouseButton::RIGHT);
    tray.addClickCallback(Tray::MouseButton::LEFT, [] {
        battery::log::info("Left mouse button");
    });
    tray.addClickCallback(Tray::MouseButton::RIGHT, [] {
        battery::log::info("Right mouse button");
    });

    // Here define the options for the context menu. You don't have to define anything.
    tray.addEntry(Tray::Button("Quit", [&tray] { tray.exit(); }));
    tray.addEntry(Tray::Button("Disabled button"))->setDisabled(true);
    tray.addEntry(Tray::Separator());
    tray.addEntry(Tray::Label("Just a label"));
    tray.addEntry(Tray::Toggle("Animate", true, [&animate](bool state) {
        if (state && !animate) {
            battery::log::info("Start animation");
            animate = true;
        }
        if (!state && animate) {
            battery::log::info("Stop animation");
            animate = false;
        }
    }));
    tray.addEntry(Tray::Separator());
    tray.addEntry(Tray::Submenu("Test Submenu"))->addEntry(Tray::Button("Submenu button!"))->setDisabled(true);

    // And start the loop :)
    int i = 0;
    while (tray.run_nonblocking()) {
        tray.setIcon(animate ? frames[i++] : defaultIcon);
        i %= frames.size();
        b::sleep_ms(20);
    }

    return 0;
}
