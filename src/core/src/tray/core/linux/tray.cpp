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
#ifdef __linux__

#include <stdexcept>
#include <libappindicator/app-indicator.h>

#include "battery/core/tray/core/linux/tray.hpp"
#include "battery/core/tray/components/label.hpp"
#include "battery/core/tray/components/button.hpp"
#include "battery/core/tray/components/toggle.hpp"
#include "battery/core/tray/components/submenu.hpp"
#include "battery/core/tray/components/separator.hpp"
#include "battery/core/tray/components/imagebutton.hpp"
#include "battery/core/tray/components/syncedtoggle.hpp"

Tray::Tray::Tray(std::string identifier, std::string tooltip, MouseButton clickAction)
    : BaseTray(std::move(identifier), std::move(tooltip), std::move(clickAction))
{
    if (gtk_init_check(nullptr, nullptr) != TRUE) {
        throw std::runtime_error("Gtk init check failed");
        return;
    }

    appIndicator = app_indicator_new(this->identifier.c_str(), this->icon, APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(appIndicator, APP_INDICATOR_STATUS_ACTIVE);
}

void Tray::Tray::exit() {
    g_idle_add(
        [](gpointer data) -> gboolean {
            auto *tray = reinterpret_cast<Tray *>(data);
            g_object_unref(tray->appIndicator);
            tray->appIndicator = nullptr;
            return FALSE;
        },
        this);
}

void Tray::Tray::update() {
    if (appIndicator) {
        app_indicator_set_menu(appIndicator, reinterpret_cast<GtkMenu *>(construct(entries, this)));
    }
}

GtkMenuShell *Tray::Tray::construct(const std::vector<std::shared_ptr<TrayEntry>> &entries, Tray *parent) {
    auto *menu = reinterpret_cast<GtkMenuShell *>(gtk_menu_new());

    for (const auto &entry : entries) {
        auto *item = entry.get();
        GtkWidget *gtkItem = nullptr;

        if (auto *toggle = dynamic_cast<Toggle *>(item); toggle) {
            gtkItem = gtk_check_menu_item_new_with_label(toggle->getText().c_str());
            gtk_check_menu_item_set_active(reinterpret_cast<GtkCheckMenuItem *>(gtkItem), toggle->isToggled());
        }
        else if (auto *syncedToggle = dynamic_cast<SyncedToggle *>(item); syncedToggle) {
            gtkItem = gtk_check_menu_item_new_with_label(syncedToggle->getText().c_str());
            gtk_check_menu_item_set_active(reinterpret_cast<GtkCheckMenuItem *>(gtkItem), syncedToggle->isToggled());
        }
        else if (auto *submenu = dynamic_cast<Submenu *>(item); submenu) {
            gtkItem = gtk_menu_item_new_with_label(submenu->getText().c_str());
            gtk_menu_item_set_submenu(reinterpret_cast<GtkMenuItem *>(gtkItem),
                                      reinterpret_cast<GtkWidget *>(construct(submenu->getEntries(), parent)));
        }
        else if (auto *iconButton = dynamic_cast<ImageButton *>(item); iconButton) {
            gtkItem = gtk_menu_item_new();

            GtkWidget *image = iconButton->getImage();
            auto *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
            auto *label = gtk_label_new(iconButton->getText().c_str());

            bool handled = false;
            if (parent) {
                for (std::size_t i = 0; parent->imageWidgets.size() > i; i++) {
                    const auto &[container, widget] = parent->imageWidgets.at(i);

                    if (widget == image) {
                        g_object_ref(widget); // NOLINT

                        gtk_container_remove(container, widget);
                        gtk_container_add(reinterpret_cast<GtkContainer *>(box),
                                          widget); // TODO(performance): This takes ages - find a way to improve it.

                        parent->imageWidgets.erase(parent->imageWidgets.begin() + i);
                        handled = true;
                        break;
                    }
                }
            }

            if (!handled) {
                gtk_container_add(reinterpret_cast<GtkContainer *>(box), image);
            }

            gtk_label_set_xalign(reinterpret_cast<GtkLabel *>(label), 0.0);
            gtk_box_pack_end(reinterpret_cast<GtkBox *>(box), label, TRUE, TRUE, 0);

            gtk_container_add(reinterpret_cast<GtkContainer *>(gtkItem), box);

            if (parent) {
                parent->imageWidgets.emplace_back(reinterpret_cast<GtkContainer *>(box),
                                                  reinterpret_cast<GtkWidget *>(image));
            }
        }
        else if (dynamic_cast<Button *>(item)) {
            gtkItem = gtk_menu_item_new_with_label(item->getText().c_str());
        }
        else if (dynamic_cast<Label *>(item)) {
            gtkItem = gtk_menu_item_new_with_label(item->getText().c_str());
            gtk_widget_set_sensitive(gtkItem, FALSE);
        }
        else if (dynamic_cast<Separator *>(item)) {
            gtkItem = gtk_separator_menu_item_new();
        }

        if (!dynamic_cast<Label *>(item)) {
            gtk_widget_set_sensitive(gtkItem, !item->isDisabled());
        }

        g_signal_connect(gtkItem, "activate", reinterpret_cast<GCallback>(callback), item);

        gtk_widget_show(gtkItem);
        gtk_menu_shell_append(menu, gtkItem);
    }

    return menu;
}

void Tray::Tray::callback([[maybe_unused]] GtkWidget *widget, gpointer data) {
    auto *item = reinterpret_cast<TrayEntry *>(data);

    if (auto *button = dynamic_cast<Button *>(item); button) {
        button->clicked();
    }
    else if (auto *toggle = dynamic_cast<Toggle *>(item); toggle) {
        toggle->onToggled();
    }
    else if (auto *syncedToggle = dynamic_cast<SyncedToggle *>(item); syncedToggle) {
        syncedToggle->onToggled();
    }
}

void Tray::Tray::run() {
    while (appIndicator) {
        gtk_main_iteration_do(true);
    }
}

#endif // __linux__
#endif // BATTERY_CORE_NO_TRAY
