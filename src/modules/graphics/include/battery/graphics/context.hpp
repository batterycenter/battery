#pragma once

#include "battery/python/python.hpp"

namespace b {

    template <typename Derived, typename Base>
    concept derived_from = std::is_base_of_v<Base, Derived>;

    struct context {
        virtual void define_python_types(b::py::module& module) = 0;
    };

    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char (&str)[N]) { std::copy_n(str, N, value); }
        char value[N];
    };

}