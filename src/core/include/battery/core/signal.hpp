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

#include <functional>

///
/// \brief Signal handling and program termination
/// \addtogroup signal Ctrl-C handling
/// @{
///
namespace b {

    ///
    /// \brief Set a handler to be executed when the user presses Ctrl+C during execution
    /// \details This function replaces any previously set handler. Only a single handler can be set globally.
    ///          You are highly encouraged to implement a custom handler that terminates your program gracefully.
    ///          Multiple consecutive CTRL+C presses will call the handler once for each press, one after another. 
    ///          They are called from a dedicated background thread, but always from the same one.
    /// \warning The callback is called from a background thread. Be aware when accessing shared resources!
    /// \param[in] handler Any callable to be called from another thread when the user presses Ctrl+C
    ///
    void set_ctrl_c_handler(const std::function<void()>& handler);

    namespace internal {
        /// @private
        void init_ctrl_c_handler();
    }

} // namespace b
/// @}