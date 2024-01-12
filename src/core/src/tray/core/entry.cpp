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

#ifndef B_OS_WEB

#include "battery/tray/core/entry.hpp"
#include "battery/tray/core/traybase.hpp"

namespace b::tray {

    tray_entry::tray_entry(std::string text) : text(std::move(text)) {}

    basetray *tray_entry::getParent() {
        return parent;
    }

    void tray_entry::setParent(basetray *newParent) {
        parent = newParent;
    }

    std::string tray_entry::getText() const {
        return text;
    }

    void tray_entry::setText(std::string newText) {
        text = std::move(newText);
        if (parent) {
            parent->update();
        }
    }

    bool tray_entry::isDisabled() const {
        return disabled;
    }

    void tray_entry::setDisabled(bool state) {
        disabled = state;
        if (parent) {
            parent->update();
        }
    }

}

#endif // !B_OS_WEB
