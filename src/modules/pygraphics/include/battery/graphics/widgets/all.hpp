#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/widgets/checkbox.hpp"
#include "battery/graphics/widgets/checkbox_ex.hpp"
#include "battery/graphics/widgets/container.hpp"
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/widgets/button_ex.hpp"
#include "battery/graphics/widgets/grid.hpp"
#include "battery/graphics/widgets/image.hpp"
#include "battery/graphics/widgets/input.hpp"
#include "battery/graphics/widgets/rect.hpp"
#include "battery/graphics/widgets/text.hpp"
#include "battery/graphics/widgets/panel.hpp"

namespace b {

    inline void define_widget_types(b::py::module& module) {
        auto widgets = module.def_submodule("widgets");

        b::widget_style::definePythonClass(widgets);
        b::widgets::base_widget::definePythonClass(widgets);
        b::widgets::button::definePythonClass(widgets);
        b::widgets::checkbox::definePythonClass(widgets);
        b::widgets::checkbox_ex::definePythonClass(widgets);
        b::widgets::container::definePythonClass(widgets);
        b::widgets::dummies::definePythonFunctions(widgets);
        b::widgets::button_ex::definePythonClass(widgets);
        b::widgets::grid::definePythonClass(widgets);
        b::widgets::image::definePythonClass(widgets);
        b::widgets::input::definePythonClass(widgets);
        b::widgets::rect::definePythonClass(widgets);
        b::widgets::text::definePythonClass(widgets);
        b::widgets::panel::definePythonClass(widgets);
    }

}