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

    class base64_invalid_error : public std::exception {
    public:
        base64_invalid_error() = default;

        const char* what() {    // NOLINT (readability-convert-member-functions-to-static)
            return "base64_invalid_error: Base-64 encoded resource is invalid.";
        }
    };

} // namespace b