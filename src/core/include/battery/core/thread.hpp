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
#include "battery/core/folders.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"
#include "battery/core/constants.hpp"

///
/// \brief Battery functions related to multithreading
/// \addtogroup thread
/// @{
///
namespace b {

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
        template<typename Fn, typename... Args>
            requires (!std::same_as<thread, std::remove_cvref_t<Fn>>)
        explicit thread(Fn&& function, Args&&... args)
            : std::jthread([&function,&args...,this] {
                thread::_jthread(std::forward<Fn>(function), std::forward<Args>(args)...);
            })
        {                                   // Wait until the thread has properly started. This caused some very weird
            promise.get_future().get();     // issues with spurious segmentation faults because of broken references.
        }                                   // This seems to fix it.

        ///
        /// \brief Run a function and catch all exceptions with nice error messages.
        /// \details Just specify a large lambda, containing everything to execute. When any exception is not handled and caught,
        ///          an error is printed with an optional error message box. See `b::thread::enable_message_box_on_exception()`
        ///          and `b::thread::enable_catch_common_exceptions()` to set this behaviour. If the message box is disabled,
        ///          only the error message is printed to the console, if catch_common_exceptions is disabled, no exception
        ///          is caught at all. Even though the main thread is not a `b::thread`, the main thread is also impacted by
        ///          these settings. Although this function is used mainly internally, you could also use this function
        ///          to create your own safe thread.
        ///
        ///          Defaults: In Debug mode, both settings are enabled, in Release mode only the message box is disabled.
        /// \param[in] func The lambda function to execute safely
        /// \see b::thread::enable_message_box_on_exception()
        /// \see b::thread::enable_catch_common_exceptions()
        ///
        template<class... Args>
        inline static void catch_common_exceptions(Args&&... args) {
            if (!_catch_common_exceptions) {            // Catching is disabled: Call and return
                std::invoke(std::forward<Args>(args)...);
                return;
            }

            try {                                       // Otherwise catch it properly
                std::invoke(std::forward<Args>(args)...);
            }
            catch (const std::exception& e) {
                battery::log::core::critical("Unhandled exception in b::thread: [std::exception]: {}", e.what());
                if (_message_box_on_exception) {
                    battery::message_box_error(fmt::format("Unhandled exception in b::thread: [std::exception]: {}", e.what()));
                }
            }
            catch (...) {
                battery::log::core::critical("Unidentifiable exception caught in b::thread, no further information");
                if (_message_box_on_exception) {
                    battery::message_box_error("Unidentifiable exception caught in b::thread, no further information");
                }
            }
        }

        ///
        /// \brief Specify the behaviour for `b::thread::catch_common_exceptions()`.
        /// \details Please check `b::thread::catch_common_exceptions()` for an explanation.
        /// \param[in] enable if a message box should appear when an exception is caught
        /// \see b::thread::catch_common_exceptions()
        ///
        inline static void enable_message_box_on_exception(bool enable) {
            _message_box_on_exception = enable;
        }

        ///
        /// \brief Specify the behaviour for `b::thread::catch_common_exceptions()`.
        /// \details Please check `b::thread::catch_common_exceptions()` for an explanation.
        /// \param[in] enable if exceptions should be caught at all
        /// \see b::thread::catch_common_exceptions()
        ///
        inline static void enable_catch_common_exceptions(bool enable) {
            _catch_common_exceptions = enable;
        }

    private:
        template<typename Fn, typename... Args>
        void _jthread(Fn&& function, Args&&... args) {
            catch_common_exceptions([&function,&args...,this] {
                promise.set_value(true);                                // Signalize the thread is ready
                std::invoke(std::forward<Fn>(function), std::forward<Args>(args)...);
            });
        }

        std::promise<bool> promise;
        inline static std::atomic<bool> _message_box_on_exception { b::constants::message_box_on_exception_default() };
        inline static std::atomic<bool> _catch_common_exceptions { b::constants::catch_common_exceptions_default() };
    };

}