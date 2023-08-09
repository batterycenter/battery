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

#ifdef B_OS_WINDOWS

#include "battery/core/string.hpp"
#include "battery/core/resource.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/tray/core/entry.hpp"
#include "battery/core/tray/core/windows/tray.hpp"
#include "battery/core/tray/components/button.hpp"
#include "battery/core/tray/components/label.hpp"
#include "battery/core/tray/components/separator.hpp"
#include "battery/core/tray/components/submenu.hpp"
#include "battery/core/tray/components/syncedtoggle.hpp"
#include "battery/core/tray/components/toggle.hpp"

#include <stdexcept>
#include <Windows.h>

namespace b::tray {

    static constexpr auto WM_TRAY = WM_USER + 1;

    static HICON loadIcon(const b::Resource& icon) {
        return CreateIconFromResourceEx(std::bit_cast<PBYTE>(icon.data()), icon.size(), true, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
    }

    template<typename T>
    static HINSTANCE registerClass(T wndProc, b::string identifier) {
        auto nativeIdentifier = identifier.encode_native();

        WNDCLASSEX windowClass;
        memset(&windowClass, 0, sizeof(windowClass));
        windowClass.cbSize = sizeof(windowClass);
        windowClass.lpfnWndProc = wndProc;
        windowClass.lpszClassName = nativeIdentifier.c_str();
        windowClass.hInstance = GetModuleHandle(nullptr);

        if (RegisterClassEx(&windowClass) == 0) {
            throw std::runtime_error("Failed to register class");
        }
        return windowClass.hInstance;
    }

    tray::tray(b::string identifier, b::string tooltip, MouseButton clickAction)
            : basetray(std::move(identifier), std::move(tooltip), clickAction) {
        auto windowClass = registerClass(wndProc, getIdentifier());

        // NOLINTNEXTLINE
        m_hwnd = CreateWindow(getIdentifier().encode_native().c_str(), nullptr, 0, 0, 0, 0, 0, nullptr, nullptr,
                              windowClass, nullptr);
        if (m_hwnd == nullptr) {
            throw std::runtime_error("Failed to create window");
        }
        if (UpdateWindow(m_hwnd) == 0) {
            throw std::runtime_error("Failed to update window");
        }

        auto nativeTooltip = getTooltip().encode_native();
        std::memset(&m_notifyData, 0, sizeof(m_notifyData));
        m_notifyData.cbSize = sizeof(m_notifyData);
        m_notifyData.hWnd = m_hwnd;
        lstrcpyW(m_notifyData.szTip, nativeTooltip.c_str());
        m_notifyData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        m_notifyData.uCallbackMessage = WM_TRAY;
        m_notifyData.hIcon = loadIcon(b::Resource::FromBase64(b::Constants::BatteryIconBase64()));

        if (Shell_NotifyIcon(NIM_ADD, &m_notifyData) == FALSE) {
            throw std::runtime_error("Failed to register tray icon");
        }
        trayList.insert({m_hwnd, *this});
    }

    void tray::exit() {
        Shell_NotifyIcon(NIM_DELETE, &m_notifyData);
        DestroyIcon(m_notifyData.hIcon);
        DestroyMenu(m_menu);

        UnregisterClass(getIdentifier().encode_native().c_str(), GetModuleHandle(nullptr));
        PostMessage(m_hwnd, WM_QUIT, 0, 0);

        DestroyIcon(m_notifyData.hIcon);
        trayList.erase(m_hwnd);
    }

    void tray::update() {
        DestroyMenu(m_menu);
        m_menu = construct(getEntries(), this);

        if (Shell_NotifyIcon(NIM_MODIFY, &m_notifyData) == FALSE) {
            throw std::runtime_error("Failed to update tray icon");
        }
        SendMessage(m_hwnd, WM_INITMENUPOPUP, std::bit_cast<WPARAM>(m_menu), 0);
    }

    HMENU tray::construct(const std::vector<std::shared_ptr<tray_entry>> &entries, tray *parent) {
        static auto id = 0;

        auto *menu = CreatePopupMenu();
        for (const auto &entry: entries) {
            tray_entry *item = entry.get();

            auto nativeName = item->getText().encode_native();
            MENUITEMINFO winItem{0};

            winItem.wID = ++id;
            winItem.dwTypeData = std::bit_cast<LPWSTR>(nativeName.c_str());
            winItem.cbSize = sizeof(winItem);
            winItem.fMask = MIIM_TYPE | MIIM_STATE | MIIM_DATA | MIIM_ID;
            winItem.dwItemData = std::bit_cast<ULONG_PTR>(item);

            if (const auto *toggle = dynamic_cast<class toggle*>(item); toggle) {
                if (toggle->isToggled()) {
                    winItem.fState |= MFS_CHECKED;
                } else {
                    winItem.fState |= MFS_UNCHECKED;
                }
            } else if (const auto *syncedToggle = dynamic_cast<synced_toggle*>(item); syncedToggle) {
                if (syncedToggle->isToggled()) {
                    winItem.fState |= MFS_CHECKED;
                } else {
                    winItem.fState |= MFS_UNCHECKED;
                }
            } else if (auto *submenu = dynamic_cast<class submenu*>(item); submenu) {
                winItem.fMask |= MIIM_SUBMENU;
                winItem.hSubMenu = construct(submenu->getEntries(), parent);
            } else if (const auto *label = dynamic_cast<class label*>(item); label) {
                winItem.fState = MFS_DISABLED;
            } else if (const auto *separator = dynamic_cast<class separator*>(item); separator) {
                winItem.fType = MFT_SEPARATOR;
            }

            if (const auto *label = dynamic_cast<class label*>(item); !label) {
                if (item->isDisabled()) {
                    winItem.fState = MFS_DISABLED;
                }
            }

            InsertMenuItem(menu, id, TRUE, &winItem);
        }

        return menu;
    }

    static bool isMouseButtonPressed(MouseButton button, LPARAM lParam) {
        switch (button) {
            using
            enum MouseButton;
            case LEFT:
                return lParam == WM_LBUTTONUP;
            case RIGHT:
                return lParam == WM_RBUTTONUP;
            case BOTH:
                return lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP;
        }
        return false;
    }

    LRESULT CALLBACK tray::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_TRAY: {
                auto &menu = trayList.at(hwnd).get();
                for (const auto &[button, callback]: menu.getClickCallbacks()) {
                    if (isMouseButtonPressed(button, lParam) && callback) {
                        callback();
                    }
                }
                if (isMouseButtonPressed(menu.getClickAction(), lParam)) {
                    POINT p;
                    GetCursorPos(&p);
                    SetForegroundWindow(hwnd);
                    auto cmd = TrackPopupMenu(menu.m_menu, TPM_RETURNCMD | TPM_NONOTIFY, p.x, p.y, 0, hwnd, nullptr);
                    SendMessage(hwnd, WM_COMMAND, cmd, 0);
                    return 0;
                }
            }
                break;
            case WM_COMMAND: {
                MENUITEMINFO winItem{0};
                winItem.fMask = MIIM_DATA | MIIM_ID;
                winItem.cbSize = sizeof(MENUITEMINFO);
                auto &menu = trayList.at(hwnd).get();
                if (GetMenuItemInfo(menu.m_menu, static_cast<UINT>(wParam), FALSE, &winItem)) {
                    auto *item = std::bit_cast<tray_entry *>(winItem.dwItemData);
                    if (auto *button = dynamic_cast<class button*>(item); button) {
                        button->clicked();
                    } else if (auto *toggle = dynamic_cast<class toggle*>(item); toggle) {
                        toggle->onToggled();
                        menu.update();
                    } else if (auto *syncedToggle = dynamic_cast<class synced_toggle*>(item); syncedToggle) {
                        syncedToggle->onToggled();
                        menu.update();
                    }
                }
            }
                break;
            default:
                break;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void tray::setIcon(const b::Resource& icon) {
        DestroyIcon(m_notifyData.hIcon);
        m_notifyData.hIcon = loadIcon(icon);
        update();
    }

    void tray::run() {
        MSG msg;
        while (GetMessage(&msg, m_hwnd, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool tray::run_nonblocking() {
        for(int i = 0; i < 5; i++) {        // We might have multiple events in a single iteration
            MSG msg;
            PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE);
            if (msg.message == WM_QUIT) {
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

}

#endif // B_OS_WINDOWS
