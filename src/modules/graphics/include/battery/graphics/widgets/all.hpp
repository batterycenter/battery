#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/widgets/checkbox.hpp"
#include "battery/graphics/widgets/checkbox_ex.hpp"
#include "battery/graphics/widgets/container.hpp"
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/widgets/fancy_button.hpp"
#include "battery/graphics/widgets/grid.hpp"
#include "battery/graphics/widgets/image.hpp"
#include "battery/graphics/widgets/input.hpp"
#include "battery/graphics/widgets/rect.hpp"
#include "battery/graphics/widgets/text.hpp"
#include "battery/graphics/widgets/window.hpp"

namespace b {

    inline void define_widget_types(b::py::module& module) {
        auto widgets = module.def_submodule("widgets");

        b::widget_style::define_python_types(widgets);
        b::widgets::base_widget::define_python_types(widgets);
        b::widgets::button::define_python_types(widgets);
        b::widgets::checkbox::define_python_types(widgets);
        b::widgets::checkbox_ex::define_python_types(widgets);
        b::widgets::container::define_python_types(widgets);
        b::widgets::dummies::define_python_types(widgets);
        b::widgets::fancy_button::define_python_types(widgets);
        b::widgets::grid::define_python_types(widgets);
        b::widgets::image::define_python_types(widgets);
        b::widgets::input::define_python_types(widgets);
        b::widgets::rect::define_python_types(widgets);
        b::widgets::text::define_python_types(widgets);
        b::widgets::window::define_python_types(widgets);
    }

}