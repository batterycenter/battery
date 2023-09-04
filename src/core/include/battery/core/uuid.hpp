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

#include "battery/core/string.hpp"

///
/// \brief Everything related to uuids
/// \addtogroup Uuid
/// @{
///
namespace b::uuid {

    ///
    /// \brief Generate a new uuid in the form of a string.
    /// \warning This function is not crypthographically secure. It is only meant to be used for local id generation.
    ///          Do not use it for anything security or network related.
    /// \return A new, unique uuid in the form of an b::string
    /// \see b::uuid::is_valid()
    ///
    std::string v4();

    ///
    /// \brief Test if a string is in the correct format to be interpreted as a uuid.
    /// \param[in] uuid The string to test
    /// \return True if the string is a valid uuid, false otherwise
    /// \see b::uuid::v4()
    ///
    bool is_valid(const std::string& uuid);

} // namespace b::uuid
