#pragma once

#include "battery/python/python.hpp"

namespace b::widgets {

    void vspace(double height);
    void hspace(double width);
    void dummy(double width, double height);

    namespace dummies {
        inline void definePythonFunctions(b::py::module& module) {
            module.def("vspace", &b::widgets::vspace);
            module.def("hspace", &b::widgets::hspace);
            module.def("dummy", &b::widgets::dummy);
        }
    }

}