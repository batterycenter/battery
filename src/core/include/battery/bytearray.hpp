#ifndef BATTERY_CORE_BYTEARRAY_HPP
#define BATTERY_CORE_BYTEARRAY_HPP

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

#include <string>
#include <vector>
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ranges.h"
#include "spdlog/fmt/std.h"

namespace b {

    using bytearray = std::vector<uint8_t>;

    /// \brief A higher-level byte array class to be used for binary data
    /// \details This class is a wrapper around std::vector<uint8_t>. The goal is to provide a standalone type for
    ///          binary data, such as binary files, etc. It goes hand-in-hand with the b::string class and the
    ///          filesystem functions. The end goal is that resources are stored as either b::string if they represent
    ///          text, or b::bytearray if they represent binary data (or a text with a specific encoding). Binary data
    ///          should use this class instead of std::string, potentially containing non-printable characters. It
    ///          can also be conveniently formatted and printed.
//    class bytearray : public std::vector<uint8_t> {
//    public:
//        /// \brief Construct an empty byte array
//        bytearray() = default;
//
//        /// \brief Construct a byte array from a vector of bytes
//        bytearray(const std::vector<uint8_t>& vec) : std::vector<uint8_t>(vec) {}
//
//        /// \brief Construct a byte array from a vector of bytes
//        bytearray(std::vector<uint8_t>&& vec) : std::vector<uint8_t>(vec) {}
//
//        /// \brief Construct a byte array from a list of bytes
//        bytearray(std::initializer_list<uint8_t> list) : std::vector<uint8_t>(list) {}
//
//        static bytearray from_string(const std::string& str) {
//            return { std::vector<uint8_t>(str.begin(), str.end()) };
//        }
//
//        std::string str() const {
//            return { begin(), end() };
//        }
//
//        /// \brief Assign this byte array to another byte array
//        bytearray& operator=(const std::vector<uint8_t>& vec) {
//            std::vector<uint8_t>::operator=(std::vector<uint8_t>(vec));
//            return *this;
//        }
//
//        /// \brief Assign this byte array to a vector of bytes
//        bytearray& operator=(std::vector<uint8_t>&& vec) {
//            std::vector<uint8_t>::operator=(std::vector<uint8_t>(vec));
//            return *this;
//        }
//
//        /// \brief Assign this byte array to a list of bytes
//        bytearray& operator=(std::initializer_list<uint8_t> list) {
//            std::vector<uint8_t>::operator=(std::vector<uint8_t>(list));
//            return *this;
//        }
//    };

} // namespace b

///// \brief fmt formatter to allow formatting b::bytearray with fmt::format
//template <> struct fmt::formatter<b::bytearray> {
//    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
//        return ctx.begin();
//    }
//    auto format(const b::bytearray& array, format_context& ctx) const -> format_context::iterator {
//        return fmt::format_to(ctx.out(), "{}", static_cast<std::vector<uint8_t>>(array));
//    }
//};

#endif // BATTERY_CORE_BYTEARRAY_HPP