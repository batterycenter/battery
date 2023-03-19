//
// Copyright 2022 Florian Zachs (HerrNamenlos123)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was part of the "traypp library" found at https://github.com/Soundux/traypp.
// The library was originally released under the MIT license.
// It has been heavily modified and modernized to work with battery.
//

#ifndef BATTERY_CORE_NO_TRAY

#include "battery/core/environment.hpp"
#ifndef BATTERY_ARCH_WINDOWS

#include <stdexcept>
#include <libayatana-appindicator/app-indicator.h>

#include "battery/core/tray/core/linux/tray.hpp"
#include "battery/core/tray/components/label.hpp"
#include "battery/core/tray/components/button.hpp"
#include "battery/core/tray/components/toggle.hpp"
#include "battery/core/tray/components/submenu.hpp"
#include "battery/core/tray/components/separator.hpp"
#include "battery/core/tray/components/syncedtoggle.hpp"

#include "battery/core/fs.hpp"
#include "battery/core/time.hpp"
#include "battery/core/constants.hpp"

namespace b::tray {

    struct tray::tray_data {
        AppIndicator *appIndicator;
        std::vector<std::pair<GtkContainer *, GtkWidget *>> imageWidgets;
        b::resource::on_disk_resource iconfile;     // Apparently the icon is loaded at a later time, so we have to keep
                                                    // the resource alive (The file on disk must not be deleted too early)
        static void callback(GtkWidget *, gpointer);
        static GtkMenuShell *construct(const std::vector<std::shared_ptr<tray_entry>> &entries, tray *parent);
    };

    void tray::tray_data::callback([[maybe_unused]] GtkWidget *widget, gpointer data) {
        auto *item = reinterpret_cast<tray_entry*>(data);

        if (auto *button = dynamic_cast<class button*>(item); button) {
            button->clicked();
        } else if (auto *toggle = dynamic_cast<class toggle*>(item); toggle) {
            toggle->onToggled();
        } else if (auto *syncedToggle = dynamic_cast<class synced_toggle*>(item); syncedToggle) {
            syncedToggle->onToggled();
        }
    }

    GtkMenuShell *tray::tray_data::construct(const std::vector<std::shared_ptr<tray_entry>> &entries, tray *parent) {
        auto *menu = reinterpret_cast<GtkMenuShell *>(gtk_menu_new());

        for (const auto &entry: entries) {
            auto *item = entry.get();
            GtkWidget *gtkItem = nullptr;

            if (auto *toggle = dynamic_cast<class toggle*>(item); toggle) {
                gtkItem = gtk_check_menu_item_new_with_label(toggle->getText().c_str());
                gtk_check_menu_item_set_active(reinterpret_cast<GtkCheckMenuItem *>(gtkItem), toggle->isToggled());
            } else if (auto *syncedToggle = dynamic_cast<class synced_toggle*>(item); syncedToggle) {
                gtkItem = gtk_check_menu_item_new_with_label(syncedToggle->getText().c_str());
                gtk_check_menu_item_set_active(reinterpret_cast<GtkCheckMenuItem *>(gtkItem),
                                               syncedToggle->isToggled());
            } else if (auto *submenu = dynamic_cast<class submenu*>(item); submenu) {
                gtkItem = gtk_menu_item_new_with_label(submenu->getText().c_str());
                gtk_menu_item_set_submenu(reinterpret_cast<GtkMenuItem *>(gtkItem),
                                          reinterpret_cast<GtkWidget *>(construct(submenu->getEntries(), parent)));
            } else if (dynamic_cast<class button*>(item)) {
                gtkItem = gtk_menu_item_new_with_label(item->getText().c_str());
            } else if (dynamic_cast<class label*>(item)) {
                gtkItem = gtk_menu_item_new_with_label(item->getText().c_str());
                gtk_widget_set_sensitive(gtkItem, FALSE);
            } else if (dynamic_cast<class separator*>(item)) {
                gtkItem = gtk_separator_menu_item_new();
            }

            if (!dynamic_cast<class label*>(item)) {
                gtk_widget_set_sensitive(gtkItem, !item->isDisabled());
            }

            g_signal_connect(gtkItem, "activate", reinterpret_cast<GCallback>(callback), item);

            gtk_widget_show(gtkItem);
            gtk_menu_shell_append(menu, gtkItem);
        }

        return menu;
    }

    tray::tray(std::string identifier, std::string tooltip, MouseButton clickAction)
            : basetray(std::move(identifier), std::move(tooltip), clickAction),
              data(std::make_unique<tray_data>())
    {
        if (gtk_init_check(nullptr, nullptr) != TRUE) {
            throw std::runtime_error("Gtk init check failed");
            return;
        }

        auto icon = b::resource::from_base64(b::constants::battery_icon_base64(), "png");
        auto iconfile = icon.as_temporary_on_disk_resource();
        data->appIndicator = app_indicator_new(getIdentifier().c_str(), data->iconfile.str().c_str(),
                                         APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
        app_indicator_set_status(data->appIndicator, APP_INDICATOR_STATUS_ACTIVE);
    }

    tray::~tray() {}    // Must be defined in this file (unique_ptr incomplete type)

    void tray::exit() {
        g_idle_add(
                [](gpointer data) -> gboolean {
                    auto *tray = reinterpret_cast<class tray*>(data);
                    g_object_unref(tray->data->appIndicator);
                    tray->data->appIndicator = nullptr;
                    return FALSE;
                },this);
    }

    void tray::update() {
        if (data->appIndicator) {
            app_indicator_set_menu(data->appIndicator, reinterpret_cast<GtkMenu*>(data->construct(getEntries(), this)));
        }
    }

    void tray::setIcon(const b::resource& icon) {
        data->iconfile = icon.as_temporary_on_disk_resource();
        app_indicator_set_icon(data->appIndicator, data->iconfile.str().c_str());
    };

    void tray::run() {
        while (data->appIndicator) {
            gtk_main_iteration_do(true);
        }
    }

    bool tray::run_nonblocking() {
        for(int i = 0; i < 5; i++) {        // We might have multiple events in a single iteration
            gtk_main_iteration_do(false);
        }
        return data->appIndicator;
    };

}

#endif // !BATTERY_ARCH_WINDOWS
#endif // BATTERY_CORE_NO_TRAY
