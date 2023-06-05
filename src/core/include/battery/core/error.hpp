#pragma once

#include <exception>
#include "battery/core/string.hpp"

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

    b::string strerror(int errnum);

}
