#ifndef BATTERY_CORE_FORMAT_HPP
#define BATTERY_CORE_FORMAT_HPP

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

#pragma GCC diagnostic push // Ignore deprecation warnings from 'fmt' library
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ranges.h"
#include "spdlog/fmt/std.h"

#pragma GCC diagnostic pop

namespace b {

/// \brief Format a string using Python formatting syntax
/// \details This function uses fmt::format under the hood. See
/// https://fmt.dev/latest/syntax.html for more info.
///          Example usage: `b::format("Hello, {0}!", "world")`. The format string
///          must be a plain string literal, the arguments can be any type that
///          provides a formatter for fmt::format. Custom types can be formatted by
///          providing a specialization of fmt::formatter. See
///          https://fmt.dev/latest/api.html#formatting-user-defined-types
/// \param fmt The format string
/// \param args The arguments to format
/// \return The formatted string
using fmt::format;

} // namespace b

#endif // BATTERY_CORE_FORMAT_HPP
