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

#pragma once

#include <future>
#include <string>
#include <thread>
#include "battery/core/platform.h"
#include "battery/core/messages.h"
#include "battery/core/log.h"
#include "battery/core/time.h"

///
/// \brief Battery functions related to multithreading
/// \addtogroup thread
/// @{
///
namespace b {

    namespace internal {
        inline std::atomic<bool> message_box_on_exception = true;
    }

    ///
    /// \brief Specify the behaviour for `b::catch_common_exceptions()`.
    /// \details `b::catch_common_exceptions()` is also used in the `b::thread`. With this function you can specify if
    ///          an OS-level error message box should pop up when an unhandled exception is caught. The exception is
    ///          always caught and printed to the console, but the message box is optional. Default is true.
    /// \param[in] enable if a message box should appear
    /// \see b::catch_common_exceptions()
    ///
    inline void message_box_on_exception(bool enable) {
        internal::message_box_on_exception = enable;
    }

    ///
    /// \brief Run a function and catch all exceptions with nice error messages.
    /// \details Just specify a large lambda, containing everything to execute. When any exception is not handled and caught,
    ///          an error is printed if possible, with an optional message box (see `b::message_box_on_exception()`).
    ///          This function is used internally, but could also be used elsewhere. Only use this in the root of a thread!
    /// \param[in] func The lambda function to execute safely
    /// \see b::message_box_on_exception()
    ///
    template<class... Args>
    void catch_common_exceptions(Args&&... args) {
        try {
            std::invoke(std::forward<Args>(args)...);
        }
        catch (const std::exception& e) {
            battery::log::core::critical("Unhandled exception in b::thread: [std::exception]: {}", e.what());
            if (internal::message_box_on_exception) {
                battery::message_box_error(fmt::format("Unhandled exception in b::thread: [std::exception]: {}", e.what()));
            }
        }
        catch (...) {
            battery::log::core::critical("Unidentifiable exception caught in b::thread, no further information");
            if (internal::message_box_on_exception) {
                battery::message_box_error("Unidentifiable exception caught in b::thread, no further information");
            }
        }
    }

    ///
    /// \brief A threading class just like good old `std::thread`, but better.
    /// \details This class is derived from `std::jthread`, which has a few improvements over `std::thread`: It is
    ///          automatically joined in the destructor and it has the capability of being interrupted by another thread.
    ///          What `b::thread` does additionally is to simply catch any unhandled exceptions in the root of the thread.
    ///          This way you get clear error messages that something has failed, instead of your program randomly crashing.
    ///
    ///          Example when calling a class member function: `b::thread(&MyClass::myMember, this)`
    ///
    class thread : public std::jthread {
    public:
        thread() = default;

        ///
        /// \brief b::thread constructor
        /// \param[in] function The function to execute in the thread
        /// \param[in] args Any number of parameters to pass to the function when calling it
        ///
        template<class... Args>
        thread(Args&&... args)
            : std::jthread([&args...,this]() {
                promise.set_value(true);                // Apparently we have to wait until the thread is running
                b::catch_common_exceptions([&args...] {
                    std::invoke(std::forward<Args>(args)...);
                    });
                }
            )
        {
            promise.get_future().get();
        }

    private:
        std::promise<bool> promise;
    };

}