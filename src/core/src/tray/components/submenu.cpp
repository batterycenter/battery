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

#include "battery/core/tray/core/entry.hpp"
#include "battery/core/tray/components/submenu.hpp"

namespace b::tray {

    submenu::submenu(b::string text)
            : tray_entry(std::move(text)) {}

    void submenu::update() {
        if (parent) {
            parent->update();
        }
    }

    std::vector<std::shared_ptr<tray_entry>> submenu::getEntries() {
        return entries;
    }

}
