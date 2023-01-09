#pragma once

#include <string>
#include <thread>
#include "battery/core/exception.h"
#include "battery/core/platform.h"
#include "battery/core/log.h"

namespace battery {

    class thread : public std::thread {
    public:
        thread() = default;

        template <class _Fn, class... _Args, std::enable_if_t<!std::is_same_v<std::_Remove_cvref_t<_Fn>, thread>, int> = 0>
        explicit thread(_Fn&& _Fx, _Args&&... _Ax) :
            std::thread([&] {

                try {
                    return _Fx(_Ax...);
                }
                catch (const battery::exception& e) {
                    battery::log::core::critical("[battery::exception in battery::thread]: {}", e.what());
                    battery::message_box_error(std::format("[battery::exception in battery::thread]: {}", e.what()));
                }
                catch (const std::exception& e) {
                    battery::log::core::critical("[std::exception in battery::thread]: {}", e.what());
                    battery::message_box_error(std::format("[std::exception in battery::thread]: {}", e.what()));
                }
                catch (...) {
                    battery::log::core::critical("Unidentifiable exception caught in battery::thread, no further information");
                    battery::message_box_error("Unidentifiable exception caught in battery::thread, no further information");
                }

            })
        {}
    };

}