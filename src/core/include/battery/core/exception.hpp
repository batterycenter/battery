#pragma once

#include <exception>

namespace b {

    class unicode_error : public std::exception {
    public:
        unicode_error() = default;

        const char* what() {    // NOLINT (readability-convert-member-functions-to-static)
            return "unicode_error: A sequence contains invalid Unicode sequences.";
        }
    };

} // namespace b