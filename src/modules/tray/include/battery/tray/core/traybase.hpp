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
#include <string>
#include <vector>
#include <functional>
#include <type_traits>
#include "battery/core/resource.hpp"
#include "battery/tray/core/entry.hpp"

namespace b::tray {

    enum class MouseButton {
        LEFT,
        RIGHT,
        BOTH
    };

    class basetray {
      public:
        basetray(b::string identifier, b::string tooltip, MouseButton clickAction);
        virtual ~basetray() = default;

        template <typename... T>
        void addEntries(const T&... _entries) {
            (addEntry(_entries), ...);
        }

        template <typename T, std::enable_if_t<std::is_base_of_v<tray_entry, T>> * = nullptr>
        auto addEntry(const T &entry) {
            entries.emplace_back(std::make_shared<T>(entry));
            auto back = entries.back();
            back->setParent(this);
            update();
            return std::dynamic_pointer_cast<std::decay_t<T>>(back);
        }

        void addClickCallback(MouseButton button, const std::function<void()>& callback);

        virtual void setIcon(const b::resource& icon) = 0;
        virtual void run() = 0;
        [[nodiscard]] virtual bool run_nonblocking() = 0;
        virtual void exit() = 0;
        virtual void update() = 0;
        [[nodiscard]] std::vector<std::shared_ptr<tray_entry>> getEntries() const;
        [[nodiscard]] b::string getIdentifier() const;
        [[nodiscard]] b::string getTooltip() const;
        [[nodiscard]] MouseButton getClickAction() const;
        [[nodiscard]] std::vector<std::pair<MouseButton, std::function<void()>>> getClickCallbacks() const;

    private:
        b::string identifier;
        b::string tooltip;
        MouseButton clickAction;
        std::vector<std::shared_ptr<tray_entry>> entries;
        std::vector<std::pair<MouseButton, std::function<void()>>> clickCallbacks;
    };

} // namespace Tray
