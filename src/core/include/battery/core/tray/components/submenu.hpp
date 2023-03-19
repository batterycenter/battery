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
#include <battery/core/tray/core/entry.hpp>
#include <battery/core/tray/core/traybase.hpp>
#include <memory>
#include <type_traits>
#include <vector>

namespace Tray {

    class Submenu : public TrayEntry {
    public:
        explicit Submenu(std::string text);
        ~Submenu() override = default;

        template <typename... T>
        explicit Submenu(std::string text, const T &... entries) : Submenu(text) {
            addEntries(entries...);
        }

        template <typename... T>
        void addEntries(const T &... _entries) {
            (addEntry(_entries), ...);
        }

        template <typename T, std::enable_if_t<std::is_base_of_v<TrayEntry, T>> * = nullptr>
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
        std::vector<std::shared_ptr<TrayEntry>> getEntries();

    private:
        std::vector<std::shared_ptr<TrayEntry>> entries;
    };

} // namespace Tray
