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
#include "folders.hpp"
#include "messages.hpp"
#include "log.hpp"
#include "time.hpp"
#include "constants.hpp"

#ifndef B_OS_WEB
#ifndef __cpp_lib_jthread
#error "Your compiler does not support std::jthread. Please use a newer compiler that has C++20 support."
#endif
#endif // !B_OS_WEB

///
/// \brief Everything related to multithreading
/// \addtogroup Thread
/// @{
///
namespace b {

    struct CommonExceptionSettings {
        inline static std::atomic<bool> messageBoxOnExceptionEnabled {b::Constants::MessageBoxOnExceptionDefault() };
        inline static std::atomic<bool> catchCommonExceptionsEnabled {b::Constants::CatchCommonExceptionsDefault() };
    };

    ///
    /// \brief Specify the behaviour for `b::thread::catchCommonExceptions()`.
    /// \details Please check `b::thread::catchCommonExceptions()` for an explanation.
    /// \param[in] enable if a message box should appear when an exception is caught
    /// \see b::thread::catchCommonExceptions()
    ///
    inline static void EnableMessageBoxOnException(bool enable) {
        CommonExceptionSettings::messageBoxOnExceptionEnabled = enable;
    }

    ///
    /// \brief Specify the behaviour for `b::thread::catchCommonExceptions()`.
    /// \details Please check `b::thread::catchCommonExceptions()` for an explanation.
    /// \param[in] enable if exceptions should be caught at all
    /// \see b::thread::catchCommonExceptions()
    ///
    inline static void EnableCatchCommonExceptions(bool enable) {
        CommonExceptionSettings::catchCommonExceptionsEnabled = enable;
    }

    ///
    /// \brief Run a function and catch all exceptions with nice error messages.
    /// \details Just specify a large lambda, containing everything to execute. When any exception is not handled and caught,
    ///          an error is printed with an optional error message box. See `b::thread::enableMessageBoxOnException()`
    ///          and `b::thread::enableCatchCommonExceptions()` to set this behaviour. If the message box is disabled,
    ///          only the error message is printed to the console, if catchCommonExceptions is disabled, no exception
    ///          is caught at all. Even though the main thread is not a `b::thread`, the main thread is also impacted by
    ///          these settings. Although this function is used mainly internally, you could also use this function
    ///          to create your own safe thread.
    ///
    ///          Defaults: In Debug mode, both settings are enabled, in Release mode only the message box is disabled.
    /// \param[in] func The lambda function to execute safely
    /// \see b::thread::enableMessageBoxOnException()
    /// \see b::thread::enableCatchCommonExceptions()
    ///
    template<class... Args>
    inline static bool CatchCommonExceptions(Args&&... args) {
        if (!CommonExceptionSettings::catchCommonExceptionsEnabled) {            // Catching is disabled: Call and return
            std::invoke(std::forward<Args>(args)...);
            return true;
        }

        try {                                       // Otherwise catch exceptions properly
            std::invoke(std::forward<Args>(args)...);
            return true;
        }
        catch (const std::bad_expected_access<std::error_code>& err) {
            auto str = b::format("Unhandled exception: Bad expected access of <std::error_code>: {}",
                                 err.error().message());
            b::log::core::critical("{}", str);
            if (CommonExceptionSettings::messageBoxOnExceptionEnabled) {
                b::message_box_error(str);
            }
        }
        catch (const std::exception& err) {
            auto str = b::format("Unhandled [std::exception]: {}", err.what());
            b::log::core::critical("{}", str);
            if (CommonExceptionSettings::messageBoxOnExceptionEnabled) {
                b::message_box_error(str);
            }
        }
        catch (...) {
            auto str = b::format("Unhandled exception: Type is not of std::exception, "
                                        "no more information available");
            b::log::core::critical("{}", str);
            if (CommonExceptionSettings::messageBoxOnExceptionEnabled) {
                b::message_box_error(str);
            }
        }
        return false;
    }
    

#ifndef B_OS_WEB
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
                    if (!CatchCommonExceptions(std::move(func), get_stop_token(), std::move(args)...)) {
                        closeApplicationIfExists();
                    }
                } else {
                    if (!CatchCommonExceptions(std::move(func), std::move(args)...)) {
                        closeApplicationIfExists();
                    }
                }
            }) {
              m_waitRunningPromise.get_future().get();
          }

        ///
        /// \brief Close a b::Application instance if one exists
        /// \internal This function is used internally by b::thread
        ///
        static void closeApplicationIfExists();

    private:
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
#endif // !B_OS_WEB

} // namespace b

