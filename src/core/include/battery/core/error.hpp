#pragma once

#include "battery/core/string.hpp"
#include <exception>

/*
#define UNWRAP(result) \
    [&] () {  \
        auto opt = result;               \
        if (!opt) {      \
            return std::unexpected(opt.error()); \
        }              \
        return opt.value();               \
    }();
*/
namespace b {

    /// \brief Get the error message for the last error in an std function call.
    /// \details The error message for the current value of `errno` is returned.
    /// \return The error message.
    /// \see b::strerror(int)
    b::string strerror();

    /// \brief Get the error message for a specific error enum
    /// \details This function is commonly used in conjunction with `strerror(errno)`.
    /// \return The error message.
    /// \see b::strerror()
    b::string strerror(int errnum);

}
