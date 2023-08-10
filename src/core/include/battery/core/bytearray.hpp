#ifndef BATTERY_CORE_BYTE_HPP
#define BATTERY_CORE_BYTE_HPP

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

#include "battery/core/string.hpp"
#include <string>
#include <vector>

namespace b {

    class bytearray : public std::vector<uint8_t> {
    public:
        bytearray() = default;
        bytearray(const std::vector<uint8_t>& vec) : std::vector<uint8_t>(vec) {}
        bytearray(std::vector<uint8_t>&& vec) : std::vector<uint8_t>(std::move(vec)) {}

        bytearray(const std::string& str) : std::vector<uint8_t>(str.begin(), str.end()) {}

        bytearray& operator=(const std::string& str) {
            std::vector<uint8_t>::operator=(std::vector<uint8_t>(str.begin(), str.end()));
            return *this;
        }

        operator std::string() const {
            return std::string(begin(), end());
        }

        /// \brief Decode this byte array from UTF-8 encoding to generic b::string format
        /// \details Refer to `b::string::decode_utf8()` for more information.
        /// \return The generic, encoding-agnostic string
        /// \see b::string::decode_utf8()
        b::string decode_utf8() const {
            return b::string::decode_utf8(*this);
        }

        /// \brief Decode this byte array from plain ASCII encoding to generic b::string format
        /// \details Refer to `b::string::decode_ascii()` for more information.
        /// \return The generic, encoding-agnostic string
        /// \see b::string::decode_ascii()
        b::string decode_ascii() const {
            return b::string::decode_ascii(*this);
        }

        /// \brief Decode this byte array from Latin-1 encoding to generic b::string format.
        ///        Equivalent to `b::string::decode_iso8859_1()`.
        /// \details Refer to `b::string::decode_latin1()` for more information.
        /// \return The generic, encoding-agnostic string
        /// \see b::string::decode_latin1()
        b::string decode_latin1() const {
            return b::string::decode_latin1(*this);
        }

        /// \brief Decode this byte array from ISO-8859-1 encoding to generic b::string format.
        ///        Equivalent to `b::string::decode_latin1()`.
        /// \details Refer to `b::string::decode_iso8859_1()` for more information.
        /// \return The generic, encoding-agnostic string
        /// \see b::string::decode_iso8859_1()
        b::string decode_iso8859_1() const {
            return b::string::decode_iso8859_1(*this);
        }

#ifdef B_OS_WINDOWS
        /// \brief Decode this byte array from Windows-1252 encoding to generic b::string format.
        /// \details Refer to `b::string::decode_windows1252()` for more information.
        /// \note This function is only available on Windows.
        /// \return The generic, encoding-agnostic string
        /// \see b::string::decode_windows1252()
        b::string decode_windows1252() const {
            return b::string::decode_windows1252(*this);
        }
#endif
    };

} // namespace b

#endif // BATTERY_CORE_BYTE_HPP