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

#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "battery/tray/core/entry.hpp"
#include "battery/tray/core/traybase.hpp"

namespace b::tray {

    class submenu : public tray_entry {
    public:
        explicit submenu(b::string text);
        ~submenu() override = default;

        template <typename... T>
        explicit submenu(b::string text, const T &... entries) : submenu(text) {
            addEntries(entries...);
        }

        template <typename... T>
        void addEntries(const T &... _entries) {
            (addEntry(_entries), ...);
        }

        template <typename T, std::enable_if_t<std::is_base_of_v<tray_entry, T>> * = nullptr>
        auto addEntry(const T &entry) {
            entries.emplace_back(std::make_shared<T>(entry));
            auto back = entries.back();
            back->setParent(parent);

            if (parent) {
                parent->update();
            }

            return std::dynamic_pointer_cast<T>(back);
        }

        void update();
        std::vector<std::shared_ptr<tray_entry>> getEntries();

    private:
        std::vector<std::shared_ptr<tray_entry>> entries;
    };

} // namespace Tray
