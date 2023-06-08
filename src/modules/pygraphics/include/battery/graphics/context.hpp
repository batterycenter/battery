#pragma once

#include "battery/python/python.hpp"
#include "battery/core/map.hpp"
#include "battery/core/containers.hpp"

#define B_DEF_PY_ATTR(attr) .def_readwrite(#attr, &ContextType::attr)
#define B_DEF_PY_CLASS_PROPS(...) B_MAP(B_DEF_PY_ATTR, __VA_ARGS__)

#define B_DEF_PY_RAW_CLASS_BEGIN(classname) using ContextType = classname; b::py::class_<ContextType>(module, #classname)
#define B_DEF_PY_WIDGET_SUBCLASS_BEGIN(classname) using ContextType = classname; b::py::class_<ContextType, base_widget>(module, #classname)
#define B_DEF_PY_CONTEXT_SUBCLASS_BEGIN(classname) using ContextType = classname; b::py::class_<ContextType, PyContext>(module, #classname)

#define B_DEF_PY_RAW_CLASS(classname, ...) B_DEF_PY_RAW_CLASS_BEGIN(classname) B_DEF_PY_CLASS_PROPS(__VA_ARGS__)
#define B_DEF_PY_WIDGET_SUBCLASS(classname, ...) B_DEF_PY_WIDGET_SUBCLASS_BEGIN(classname) B_DEF_PY_CLASS_PROPS(__VA_ARGS__)
#define B_DEF_PY_CONTEXT_SUBCLASS(classname, ...) B_DEF_PY_CONTEXT_SUBCLASS_BEGIN(classname) B_DEF_PY_CLASS_PROPS(__VA_ARGS__)

#define B_DEF_PY_CONTEXT_FUNC(...) void definePythonClass(b::py::module& module) override { __VA_ARGS__; }
#define B_DEF_PY_WIDGET_CONTEXT_FUNC(...) inline static void definePythonClass(b::py::module& module) { __VA_ARGS__; }  // For widgets derived from base_widget
#define B_DEF_PY_STATIC_CONTEXT_FUNC(...) inline static void definePythonClass(b::py::module& module) { __VA_ARGS__; }  // For anything else

#define B_DEF_PY_CONTEXT(...) B_DEF_PY_CONTEXT_FUNC(B_DEF_PY_CONTEXT_SUBCLASS(__VA_ARGS__))
#define B_DEF_PY_WIDGET_CONTEXT(...) B_DEF_PY_WIDGET_CONTEXT_FUNC(B_DEF_PY_WIDGET_SUBCLASS(module, __VA_ARGS__))

namespace b {

    struct PyContext {
        virtual void definePythonClass(b::py::module& module) = 0;
        std::function<void(const b::py::function&)> m_initPyWindowFunc;

        inline static void defineParentPythonClass(b::py::module& module) {
            if (b::py::hasattr(module, "py_context")) {
                return;
            }
            b::py::class_<PyContext>(module, "py_context")
                .def_readwrite("init", &PyContext::m_initPyWindowFunc);
        }
    };

    template <size_t N>
    struct template_string_literal {
        constexpr template_string_literal(const char (&str)[N]) { std::copy_n(str, N, value); }
        char value[N];
    };

}