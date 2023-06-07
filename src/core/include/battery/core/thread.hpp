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
#include <future>
#include "battery/core/folders.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"
#include "battery/core/constants.hpp"
#include "battery/core/console_application.hpp"

#ifndef __cpp_lib_jthread
#error "Your compiler does not support std::jthread. Please use a newer compiler that has C++20 support."
#endif

///
/// \brief Everything related to multithreading
/// \addtogroup Thread
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

        template <class Fn, class... Args>
        explicit thread(Fn&& func, Args&&... args)
          : std::jthread([func = std::forward<Fn>(func), ...args = std::forward<Args>(args), this]() mutable {
                m_waitRunningPromise.set_value();
                if constexpr (std::is_invocable_v<std::decay_t<Fn>, std::stop_token, std::decay_t<Args>...>) {
                    if (!catch_common_exceptions(std::move(func), get_stop_token(), std::move(args)...)) {
                        if (b::ConsoleApplication::instanceExists()) {
                            b::ConsoleApplication::get().stopApplication();
                        }
                    }
                } else {
                    if (!catch_common_exceptions(std::move(func), std::move(args)...)) {
                        if (b::ConsoleApplication::instanceExists()) {
                            b::ConsoleApplication::get().stopApplication();
                        }
                    }
                }
            }) {
              m_waitRunningPromise.get_future().get();
          }

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
        inline static bool catch_common_exceptions(Args&&... args) {
            if (!m_catchCommonExceptionsEnabled) {            // Catching is disabled: Call and return
                std::invoke(std::forward<Args>(args)...);
                return true;
            }

            try {                                       // Otherwise catch exceptions properly
                std::invoke(std::forward<Args>(args)...);
                return true;
            }
            catch (const std::exception& e) {
                b::log::core::critical("Unhandled exception in b::thread: [std::exception]: {}", e.what());
                if (m_messageBoxOnExceptionEnabled) {
                    b::message_box_error(b::format("Unhandled exception in b::thread: [std::exception]: {}", e.what()));
                }
            }
            catch (...) {
                b::log::core::critical("Unidentifiable exception caught in b::thread, no further information");
                if (m_messageBoxOnExceptionEnabled) {
                    b::message_box_error("Unidentifiable exception caught in b::thread, no further information");
                }
            }
            return false;
        }

        ///
        /// \brief Specify the behaviour for `b::thread::catch_common_exceptions()`.
        /// \details Please check `b::thread::catch_common_exceptions()` for an explanation.
        /// \param[in] enable if a message box should appear when an exception is caught
        /// \see b::thread::catch_common_exceptions()
        ///
        inline static void enable_message_box_on_exception(bool enable) {
            m_messageBoxOnExceptionEnabled = enable;
        }

        ///
        /// \brief Specify the behaviour for `b::thread::catch_common_exceptions()`.
        /// \details Please check `b::thread::catch_common_exceptions()` for an explanation.
        /// \param[in] enable if exceptions should be caught at all
        /// \see b::thread::catch_common_exceptions()
        ///
        inline static void enable_catch_common_exceptions(bool enable) {
            m_catchCommonExceptionsEnabled = enable;
        }

    private:
        inline static std::atomic<bool> m_messageBoxOnExceptionEnabled {b::Constants::MessageBoxOnExceptionDefault() };
        inline static std::atomic<bool> m_catchCommonExceptionsEnabled {b::Constants::CatchCommonExceptionsDefault() };
        std::promise<void> m_waitRunningPromise;
    };

    ///
    /// \brief Run a function asynchronously in another thread. This is a wrapper around `std::async()`.
    /// \details The advantage of this function over b::thread is that it is more concise and not a class with a dedicated lifetime.
    ///          Use this function if you only want to run a short function with a return value in another thread, returning a `std::future`.
    /// \param[in] args anything that is callable (a function, lambda function or class member function including a 'this' pointer)
    /// \return a `std::future` that can be used to await the return value of the function
    ///
    template<typename... T>
    auto async(T&&... args) {
        return std::async(std::launch::async, std::forward<T>(args)...);
    }

} // namespace b
