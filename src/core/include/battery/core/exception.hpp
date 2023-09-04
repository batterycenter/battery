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
                m_message("b::out_of_range_error") {}

        explicit out_of_range_error(const std::string& str) :
                m_message(b::format("b::out_of_range_error: {}", str)) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    class unicode_error : public std::exception {
    public:
        unicode_error() :
                m_message("b::unicode_error") {}

        explicit unicode_error(const std::string& str) :
                m_message(b::format("b::unicode_error: {}", str)) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    class base64_encoding_error : public std::exception {
    public:
        base64_encoding_error() :
                m_message("b::base64_encoding_error") {}

        explicit base64_encoding_error(const std::string& str) :
                m_message(b::format("b::base64_encoding_error: {}", str)) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    class filesystem_error : public std::exception {
    public:
        filesystem_error() :
                m_message("b::filesystem_error") {}

        explicit filesystem_error(const std::error_code& err) :
                m_message(b::format("b::filesystem_error: {}", err.message())) {}

        explicit filesystem_error(const std::string& str) :
                m_message(b::format("b::filesystem_error: {}", str)) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    class file_io_error : public std::exception {
    public:
        file_io_error() :
                m_message("b::file_io_error") {}

        explicit file_io_error(const std::error_code& err) :
                m_message(b::format("b::file_io_error: {}", err.message())) {}

        explicit file_io_error(const std::string& str) :
                m_message(b::format("b::file_io_error: {}", str)) {}

        const char* what() const {    // NOLINT (readability-convert-member-functions-to-static)
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

} // namespace b

#endif // BATTERY_CORE_EXCEPTION_HPP