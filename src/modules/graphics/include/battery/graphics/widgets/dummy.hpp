#pragma once

#include "battery/python/python.hpp"

namespace b::widgets {

    void vspace(float height);
    void hspace(float height);
    void dummy(float width, float height);

    namespace dummies {
        inline void definePythonFunctions(b::py::module& module) {
            module.def("vspace", &b::widgets::vspace);
            module.def("hspace", &b::widgets::hspace);
            module.def("dummy", &b::widgets::dummy);
        }
    }

}