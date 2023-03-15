
#include "battery/core/time.h"
#include "battery/core/tray_icon.h"
#include "battery/core/internal/platform.h"
#include "battery/core/constants.h"

#include "battery/core/extern/tray.h"

#include <iostream>

namespace battery {

    struct tray_icon::tray_data {
        struct tray tray;
        std::vector<::tray_menu> menu;
    };

    tray_icon::tray_icon() : data(std::make_unique<tray_icon::tray_data>()) {

    }

    tray_icon::~tray_icon() {
        if (isRunning) {
            destroy();
        }
    }

    tray_icon& tray_icon::get() {
        static tray_icon instance;
        return instance;
    }

    bool tray_icon::create() {
        std::scoped_lock lock(mutex);

        if (isRunning)
            return true;

        std::promise<bool> initSuccess;
        terminate = false;
        isRunning = true;
        event_loop.execute_async(          // tray_init must strictly be called in the same thread as tray_loop because win32 is very innovative
            [this,&initSuccess] {
                if (tray_init(&data->tray) < 0) {
                    battery::log::error("battery::tray_icon::create: cannot initialize");
                    initSuccess.set_value(false);
                    return;
                }
                initSuccess.set_value(true);
                this->set_icon(battery::resource::from_base64(battery::constants::BATTERY_DEFAULT_WINDOW_ICON_BASE64));

                while (!terminate) {
                    tray_loop(0);
                    battery::sleep_ms(10);
                }
            });

        auto success = initSuccess.get_future().get();  // Wait until the thread initialized
        if (!success) {                                 // (so we can return false on failure)
            terminate = true;
            event_loop.join();
            isRunning = false;
            tray_exit();
            return false;
        }

        return true;
    }

    void tray_icon::destroy() {
        std::scoped_lock lock(mutex);
        if (isRunning) {
            terminate = true;
            event_loop.join();
            isRunning = false;
            tray_exit();
        }
    }

    void tray_icon::set_icon(const battery::resource& icon) {
#ifdef BATTERY_ARCH_WINDOWS
        nid.hIcon = battery::internal::create_win32_icon_from_png(icon);    // 'nid' is a static variable from extern/tray.h
        Shell_NotifyIconW(NIM_MODIFY, &nid);
        DestroyIcon(nid.hIcon);
#endif // BATTERY_ARCH_WINDOWS
    }

    void tray_icon::update_menus() {
//        data->menu.clear();
//        for (const auto& m : menus) {
//            ::tray_menu _m;
//            _m.text = m.text;
//            _m.cb = m.cb;
//            _m.checked = m.checked;
//            _m.disabled = m.disabled;
//            _m.context = m.context;
//            data->menu.push_back();
//        }
//        data->menu[0].text = "Ludwig Meier";
//        data->menu[1].text = "Test2!";
//        data->menu[2].text = "";
//        data->tray.root_left_cb = [] {
//            battery::log::warn("Left");
//        };
//        data->tray.root_right_cb = [] {
//            battery::log::warn("Right");
//
//        };
//        data->tray.menu = data->menu.data();
//        tray_update();
    }

    void tray_icon::add_menu(const tray_menu& menu) {
        menus.emplace_back(menu);
        update_menus();
    }

    void tray_icon::clear_menus() {
        menus.clear();
        update_menus();
    }

}
