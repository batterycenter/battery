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

#include "battery/core/tray/core/traybase.hpp"

namespace b::tray {

    basetray::basetray(std::string identifier, std::string tooltip, MouseButton clickAction)
            : identifier(std::move(identifier)),
              tooltip(std::move(tooltip)),
              clickAction(clickAction) {}

    std::vector<std::shared_ptr<tray_entry>> basetray::getEntries() const {
        return entries;
    }

    std::string basetray::getIdentifier() const {
        return identifier;
    }

    std::string basetray::getTooltip() const {
        return tooltip;
    }

    MouseButton basetray::getClickAction() const {
        return clickAction;
    }

    std::vector<std::pair<MouseButton, std::function<void()>>> basetray::getClickCallbacks() const {
        return clickCallbacks;
    }

    void basetray::addClickCallback(MouseButton button, const std::function<void()> &callback) {
        clickCallbacks.emplace_back(button, callback);
    }

}

#endif // BATTERY_CORE_NO_TRAY
