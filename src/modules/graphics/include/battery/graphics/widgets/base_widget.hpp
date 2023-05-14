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
        unit_property width;
        unit_property height;
        ImVec2 actual_position;
        ImVec2 actual_size;

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
                    .def("__call__", &b::widgets::base_widget::operator());
        }

    protected:
        [[nodiscard]] std::string get_identifier() const;
        [[nodiscard]] void set_cursor_position() const;
        [[nodiscard]] ImVec2 desired_size() const;

        size_t id = 0;

    private:
        void new_id();

        inline static size_t static_id_counter = 0;
    };

}
