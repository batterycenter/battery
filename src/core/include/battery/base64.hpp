#ifndef BATTERY_CORE_BASE64_HPP
#define BATTERY_CORE_BASE64_HPP

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

#include "string.hpp"
#include "bytearray.hpp"

#include <optional>

namespace b {

    ///
    /// \brief Encode a string or text message as Base-64, encoded in UTF-8
    /// \details This function is especially useful for exchanging binary resources over a network.
    ///          See `b::encode_base64(const b::bytearray&)` for more information about binary resources.
    /// \param[in] str The string to encode
    /// \return Encoded Base-64 string
    /// \see b::decode_base64()
    ///
    std::string encode_base64(const std::string& str);

    ///
    /// \brief Encode a binary resource as Base-64
    /// \details This is especially useful for embedding small resource files in the source code, or exchanging
    ///          binary resources over a network. This overload is for encoding binary resources based on
    ///          `std::vector<uint8_t>`. Both overloads do the same thing with another input type.
    /// \param[in] str The byte-series to encode
    /// \return Encoded Base-64 string
    /// \see b::decode_base64_text()
    /// \see b::decode_base64_binary()
    ///
    std::string encode_base64(const b::bytearray& data);

    ///
    /// \brief Decode a Base-64 encoded text resource back to its original form. The encoding is assumed to be UTF-8.
    /// \details The encoding of the string before it was encoded is assumed to be UTF-8. This function is to be used
    ///          with text resources.
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \throw b::base64_invalid_error if the input string is not a valid Base-64 string or the input is empty
    /// \todo Implement a way to specify the encoding of the string before it was encoded.
    /// \return Decoded resource in its original form when it was encoded.
    /// \see b::encode_base64()
    ///
    std::optional<std::string> decode_base64_text(const std::string& str);

    ///
    /// \brief Decode a Base-64 encoded binary resource back to its original form.
    /// \details This function is to be used with binary resources such as images, icons, etc.
    /// \param[in] str A Base-64 encoded string representing a resource
    /// \throw b::base64_invalid_error if the input string is not a valid Base-64 string or the input is empty
    /// \return Decoded binary resource as a byte array
    /// \see b::encode_base64()
    ///
    std::optional<b::bytearray> decode_base64_binary(const std::string& str);

}   // namespace b

#endif // BATTERY_CORE_BASE64_HPP