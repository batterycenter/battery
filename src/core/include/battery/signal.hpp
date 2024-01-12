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

#ifndef B_OS_WEB
    ///
    /// \brief Set a handler to be executed when the user presses Ctrl+C during execution
    /// \details The callback function is pushed onto a stack and when a Ctrl+C event is received, the topmost function is called.
    ///          This can be used to handle Ctrl+C events in various stages in your application.
    ///          Multiple consecutive CTRL+C presses will call the handler once for each press, one after another.
    ///          All callbacks are called one after another from one dedicated background thread.
    /// \warning The callbacks are called from a background thread. Be aware when accessing shared resources!
    /// \param[in] handler Any callable to be called from another thread when the user presses Ctrl+C
    /// \see pop_ctrl_c_handler()
    /// \see generate_ctrl_c_handler()
    ///
    void push_ctrl_c_handler(const std::function<void()>& handler);

    ///
    /// \brief Pop the latest Ctrl+C handler set by push_ctrl_c_handler()
    /// \details Nothing happens when the function is called but no more handler is registered.
    /// \see push_ctrl_c_handler()
    /// \see generate_ctrl_c_event()
    ///
    void pop_ctrl_c_handler();

    ///
    /// \brief Generate a Ctrl+C event for the application itself
    /// \warning This function is only for rough testing and does not have 100% the same behaviour on all platforms! Do not use it for anything serious!
    /// \see push_ctrl_c_handler()
    /// \see pop_ctrl_c_handler()
    ///
    void generate_ctrl_c_event();

#endif // !B_OS_WEB

} // namespace b
