#pragma once

#include <string>
#include <thread>
#include "battery/core/exception.h"
#include "battery/core/platform.h"
#include "battery/core/log.h"

namespace battery {

    using thread = std::jthread;

    /*class thread : public std::jthread {
    public:
        thread() = default;

        explicit thread(const std::function<void(void)> func) :
            std::jthread([&] {
                try {
                    func();
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

    private:
        void worker() {
        }
    };*/

}