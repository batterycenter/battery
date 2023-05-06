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

#include "battery/core/environment.hpp"
#ifndef BATTERY_ARCH_WINDOWS

#include <string>
#include "battery/tray/core/traybase.hpp"

namespace b::tray {

    class tray : public basetray {
    public:
        tray(std::u8string identifier, std::u8string tooltip, MouseButton clickAction = MouseButton::BOTH);
        ~tray();

        virtual void setIcon(const b::resource& icon) override;
        void run() override;
        [[nodiscard]] bool run_nonblocking() override;
        void exit() override;
        void update() override;

    private:
        struct tray_data;
        std::unique_ptr<tray_data> data;
    };

} // namespace Tray

#endif // !BATTERY_ARCH_WINDOWS