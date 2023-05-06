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
#include <string>

namespace b::tray {

    class basetray;

    class tray_entry {
    public:
        explicit tray_entry(std::u8string text);        // TODO: Rework entire tray module to also take UTF-32
        virtual ~tray_entry() = default;

        basetray *getParent();
        void setParent(basetray *);

        [[nodiscard]] std::u8string getText() const;
        void setText(std::u8string);

        void setDisabled(bool);
        [[nodiscard]] bool isDisabled() const;

    protected:
        std::u8string text;
        bool disabled = false;
        basetray *parent = nullptr;
    };

} // namespace Tray
