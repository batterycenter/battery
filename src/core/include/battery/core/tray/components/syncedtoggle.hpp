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
#include <functional>
#include "battery/core/tray/core/entry.hpp"

// TODO: The synced toggle is known not to be working (At least on Linux)

namespace b::tray {

    class synced_toggle : public tray_entry {
    public:
        synced_toggle(std::string text, bool &state, std::function<void(bool&)> callback = [](const bool&){/**/});
        ~synced_toggle() override = default;

        void onToggled();
        [[nodiscard]] bool isToggled() const;

    private:
        bool &toggled;
        std::function<void(bool &)> callback;
    };

} // namespace Tray
