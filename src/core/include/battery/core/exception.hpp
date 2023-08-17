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
                m_u8Message("b::out_of_range_error") {}

        explicit out_of_range_error(const std::string& str) :
                m_u8Message(b::format("b::out_of_range_error: {}", str).encode<b::enc::utf8>()) {}

        explicit out_of_range_error(const b::string& str) :
                m_u8Message(b::format("b::out_of_range_error: {}", str).encode<b::enc::utf8>()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;    // std::string here because c_str() must return a long-living pointer
    };

    class unicode_error : public std::exception {
    public:
        unicode_error() :
            m_u8Message("b::unicode_error") {}

        explicit unicode_error(const std::string& str) :
                m_u8Message(b::format("b::unicode_error: {}", str).encode<b::enc::utf8>()) {}

        explicit unicode_error(const b::string& str) :
            m_u8Message(b::format("b::unicode_error: {}", str).encode<b::enc::utf8>()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

    class base64_encoding_error : public std::exception {
    public:
        base64_encoding_error() :
                m_u8Message("b::base64_encoding_error") {}

        explicit base64_encoding_error(const std::string& str) :
                m_u8Message(b::format("b::base64_encoding_error: {}", str).encode<b::enc::utf8>()) {}

        explicit base64_encoding_error(const b::string& str) :
                m_u8Message(b::format("b::base64_encoding_error: {}", str).encode<b::enc::utf8>()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

    class filesystem_error : public std::exception {
    public:
        filesystem_error() :
                m_u8Message("b::filesystem_error") {}

        explicit filesystem_error(const std::error_code& err) :
                m_u8Message(b::format("b::filesystem_error: {}", err.message()).encode<b::enc::utf8>()) {}

        explicit filesystem_error(const std::string& str) :
                m_u8Message(b::format("b::filesystem_error: {}", str).encode<b::enc::utf8>()) {}

        explicit filesystem_error(const b::string& str) :
                m_u8Message(b::format("b::filesystem_error: {}", str).encode<b::enc::utf8>()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

    class file_io_error : public std::exception {
    public:
        file_io_error() :
                m_u8Message("b::file_io_error") {}

        explicit file_io_error(const std::error_code& err) :
                m_u8Message(b::format("b::file_io_error: {}", err.message()).encode<b::enc::utf8>()) {}

        explicit file_io_error(const std::string& str) :
                m_u8Message(b::format("b::file_io_error: {}", str).encode<b::enc::utf8>()) {}

        explicit file_io_error(const b::string& str) :
                m_u8Message(b::format("b::file_io_error: {}", str).encode<b::enc::utf8>()) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_u8Message.c_str();
        }

    private:
        std::string m_u8Message;
    };

} // namespace b

#endif // BATTERY_CORE_EXCEPTION_HPP