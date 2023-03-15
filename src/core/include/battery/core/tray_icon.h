#pragma once

#include <string>
#include <memory>
#include "battery/core/async_worker.h"
#include "battery/core/resource.h"

namespace battery {

    class tray_icon {
    public:
        static tray_icon& get();

        struct tray_menu {
            std::string text;
            int disabled;
            int checked;

            void (*cb)(struct tray_menu *);
            void *context = nullptr;

            std::vector<tray_menu> submenus;
        };

        bool create();
        void destroy();
        void set_icon(const battery::resource& icon);
        void add_menu(const tray_menu& menu);
        void clear_menus();

    private:
        struct tray_data;       // <- Forward declaration
        std::unique_ptr<tray_data> data;
        std::mutex mutex;
        std::atomic<bool> isRunning = false;
        std::atomic<bool> terminate = false;
        battery::async_worker<void> event_loop;
        std::vector<tray_menu> menus;

    private:
        tray_icon();
        ~tray_icon();
        void update_menus();

    public:
        tray_icon(const tray_icon&) = delete;
        tray_icon(tray_icon&&) = delete;
        tray_icon& operator=(const tray_icon&) = delete;
        tray_icon& operator=(tray_icon&&) = delete;
    };

}
