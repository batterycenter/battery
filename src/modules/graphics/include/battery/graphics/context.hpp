#pragma once

#include "battery/python/python.hpp"

#define B_DEF_PY_ATTR(attr) .def_readwrite(#attr, &ContextType::attr)

#define B_DEF_PY_CLASS(module, classname, ...) \
    using ContextType = classname; \
    b::py::class_<ContextType>(module, #classname) \
        B_MAP(B_DEF_PY_ATTR, __VA_ARGS__);

#define B_DEF_PY_CONTEXT(...) \
    void define_python_types(b::py::module& module) override { \
        B_DEF_PY_CLASS(module, __VA_ARGS__) \
    }

namespace b {

    template <typename Derived, typename Base>
    concept derived_from = std::is_base_of_v<Base, Derived>;

    struct py_context {
        virtual void define_python_types(b::py::module& module) = 0;
    };

    template<size_t N>
    struct template_string_literal {
        constexpr template_string_literal(const char (&str)[N]) { std::copy_n(str, N, value); }
        char value[N];
    };

}