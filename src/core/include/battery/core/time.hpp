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

#include <cstdint>
#include <ctime>

///
/// \brief Everything related to timing
/// \addtogroup Time
/// @{
///
namespace b {

    ///
    /// \brief Get the time the program has been running in seconds (since battery was initialized).
    /// \details Can be used for all sorts of
    ///          high-level timing and measurements. Units is seconds, but the precision is dependent on the platform,
    ///          going down in the microseconds range.
    /// \return How many seconds the program has been running
    /// \see b::epoch_time()
    ///
    double time();

    ///
    /// \brief Get the epoch time in whole seconds (UNIX time).
    /// \details Use this for any further date or calendar calculations.
    /// \return UNIX time or time since epoch in seconds
    /// \see b::time()
    /// \see b::epoch_time_ms()
    /// \see b::epoch_time_us()
    ///
    std::time_t epoch_time();

    ///
    /// \brief Get the epoch time in milliseconds (UNIX time).
    /// \details Use this for any precise time calculations.
    /// \return UNIX time or time since epoch in milliseconds
    /// \see b::time()
    /// \see b::epoch_time()
    /// \see b::epoch_time_us()
    ///
    std::time_t epoch_time_ms();

    ///
    /// \brief Get the epoch time in microseconds (UNIX time).
    /// \details Use this for any precise time calculations.
    /// \return UNIX time or time since epoch in microseconds
    /// \see b::time()
    /// \see b::epoch_time()
    /// \see b::epoch_time_ms()
    ///
    std::time_t epoch_time_us();

    ///
    /// \brief Put the current thread to sleep for said seconds.
    /// \details Contrary to a busy-wait loop, this function minimizes CPU usage.
    /// \param[in] seconds time to sleep
    /// \see b::sleep_ms()
    /// \see b::sleep_precise()
    ///
    void sleep(double seconds);

    ///
    /// \brief Busy-wait for said seconds (highest possible accuracy).
    /// \details Use this function only if you need very precise timing.
    /// \warning This function does not suspend the thread. Instead it busy-waits
    ///          and thus causes full CPU usage while doing nothing.
    /// \param[in] seconds time to sleep
    /// \see b::sleep()
    /// \see b::sleep_ms()
    ///
    void sleep_precise(double seconds);

    ///
    /// \brief Put the current thread to sleep for said milliseconds.
    /// \details Contrary to a busy-wait loop, this function minimizes CPU usage.
    /// \param[in] milliseconds time to sleep
    /// \see b::sleep()
    /// \see b::sleep_precise()
    ///
    void sleep_ms(double milliseconds);

} // namespace b
