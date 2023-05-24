#pragma once

#include <string>
#include <functional>
#include <utility>
#include "battery/python/python.hpp"
#include "battery/graphics/widget_builder.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"
#include "battery/graphics/widget_style.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/types.hpp"
#include "magic_enum.hpp"

namespace b::widgets {

    // Every class derived from BasicWidget gets a unique ID,
    // which is pushed to ImGui for context separation

    class base_widget {
    public:
        std::string name;
        py::object context;

        unit_property left;
        unit_property top;
        unit_property width;        // width and height have priority over right and bottom
        unit_property height;
        unit_property right;
        unit_property bottom;

        ImVec2 actual_position;
        ImVec2 actual_size;

        widget_style style;

        explicit base_widget(py::object context, std::string name);
        virtual ~base_widget() = default;

        base_widget(base_widget const& other) = delete;     // Copying is NOT allowed due to the unique ID
        void operator=(base_widget const& other) = delete;

        base_widget(base_widget&& other) = default;	        // Moving is allowed as the other object is then invalid
        base_widget& operator=(base_widget&& other) = default;

        virtual void operator()() = 0;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::base_widget>(module, "base_widget")
                    .def_readwrite("name", &b::widgets::base_widget::name)
                    .def_readwrite("context", &b::widgets::base_widget::context)
                    .def_readwrite("left", &b::widgets::base_widget::left)
                    .def_readwrite("top", &b::widgets::base_widget::top)
                    .def_readwrite("width", &b::widgets::base_widget::width)
                    .def_readwrite("height", &b::widgets::base_widget::height)
                    .def_readwrite("right", &b::widgets::base_widget::right)
                    .def_readwrite("bottom", &b::widgets::base_widget::bottom)
                    .def_readwrite("actual_position", &b::widgets::base_widget::actual_position)
                    .def_readwrite("actual_size", &b::widgets::base_widget::actual_size)
                    .def_readwrite("style", &b::widgets::base_widget::style)
                    .def("__call__", &b::widgets::base_widget::operator());
        }

    protected:
        std::string base_get_identifier() const;
        void base_set_cursor_position_to_min_bb() const;
        std::pair<ImVec2, ImVec2> base_get_bb() const;
        ImVec2 base_get_bb_min() const;
        ImVec2 base_get_bb_max() const;
        ImVec2 base_get_bb_size() const;
        void base_push_style();
        void base_pop_style();

        size_t id = 0;

    private:
        void new_id();

        inline static size_t static_id_counter = 0;
    };

}
