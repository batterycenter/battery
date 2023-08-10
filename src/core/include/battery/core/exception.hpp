#ifndef BATTERY_CORE_EXCEPTION_HPP
#define BATTERY_CORE_EXCEPTION_HPP

#include "battery/core/format.hpp"
#include "battery/core/string.hpp"
#include <exception>
#include <expected>
#include <new>

namespace b {

    class out_of_range_error : public std::exception {
    public:
        out_of_range_error() :
                m_u8Message("out_of_range_error: An element access was out of bounds") {}

        explicit out_of_range_error(const std::string& str) :
                m_u8Message(b::format("out_of_range_error: {}", str).encode_utf8()) {}

        explicit out_of_range_error(const b::string& str) :
                m_u8Message(b::format("out_of_range_error: {}", str).encode_utf8()) {}

        template<typename... Args>
        explicit out_of_range_error(fmt::format_string<Args...> str, Args&&... args) :
                m_u8Message(b::format("out_of_range_error: {}",
                                      fmt::format(str, std::forward<Args>(args)...)).encode_utf8()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

    class unicode_error : public std::exception {
    public:
        unicode_error() :
            m_u8Message("unicode_error: A sequence contains invalid Unicode sequences.") {}

        explicit unicode_error(const std::string& str) :
                m_u8Message(b::format("unicode_error: {}", str).encode_utf8()) {}

        explicit unicode_error(const b::string& str) :
            m_u8Message(b::format("unicode_error: {}", str).encode_utf8()) {}

        template<typename... Args>
        explicit unicode_error(fmt::format_string<Args...> str, Args&&... args) :
            m_u8Message(b::format("unicode_error: {}",
                                  fmt::format(str, std::forward<Args>(args)...)).encode_utf8()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

    class base64_encoding_error : public std::exception {
    public:
        base64_encoding_error() :
                m_u8Message("base64_encoding_error: Base-64 encoded resource is not a valid resource.") {}

        explicit base64_encoding_error(const b::string& str) :
                m_u8Message(b::format("base64_encoding_error: {}", str).encode_utf8()) {}

        template<typename... Args>
        explicit base64_encoding_error(fmt::format_string<Args...> str, Args&&... args) :
                m_u8Message(b::format("base64_encoding_error: {}",
                                      fmt::format(str, std::forward<Args>(args)...)).encode_utf8()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

} // namespace b

#endif // BATTERY_CORE_EXCEPTION_HPP