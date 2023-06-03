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
#include "battery/graphics/context.hpp"
#include "magic_enum.hpp"

namespace b::widgets {

    // Every class derived from BasicWidget gets a unique ID,
    // which is pushed to ImGui for context separation

    class base_widget {
    public:
        b::string name;
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

        explicit base_widget(py::object context, b::string name);
        virtual ~base_widget() = default;

        base_widget(base_widget const& other) = delete;     // Copying is NOT allowed due to the unique ID
        void operator=(base_widget const& other) = delete;

        base_widget(base_widget&& other) = default;	        // Moving is allowed as the id will not be duplicated
        base_widget& operator=(base_widget&& other) = default;

        virtual void operator()() = 0;

        B_DEF_PY_STATIC_CONTEXT_FUNC(
            B_DEF_PY_RAW_CLASS(base_widget, name, context, left, top, width, height, right, bottom, actual_position, actual_size, style)
            .def("__call__", &base_widget::operator())
        )

    protected:
        b::string baseGetIdentifier() const;
        void baseSetCursorPositionToMinBB() const;
        std::pair<ImVec2, ImVec2> baseGetBB() const;
        ImVec2 baseGetBBMin() const;
        ImVec2 baseGetBBMax() const;
        ImVec2 baseGetBBSize() const;
        void basePushStyle();
        void basePopStyle();

        size_t m_id = 0;

    private:
        void createNewId();

        inline static size_t m_staticIdCounter = 0;
    };

}
