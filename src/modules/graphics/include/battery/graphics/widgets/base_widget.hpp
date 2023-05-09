#pragma once

#include <string>
#include <functional>
#include <utility>
#include "battery/graphics/widget_builder.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"
#include "magic_enum.hpp"

namespace b::widgets {

    // Every class derived from BasicWidget gets a unique ID,
    // which is pushed to ImGui for context separation

    class base_widget {
    public:
        std::string name;

        explicit base_widget(std::string name) ;
        virtual ~base_widget() = default;

        base_widget(base_widget const& other) = delete;     // Copying is NOT allowed due to the unique ID
        void operator=(base_widget const& other) = delete;

        base_widget(base_widget&& other) = default;	        // Moving is allowed as the other object is then invalid
        base_widget& operator=(base_widget&& other) = default;

        virtual void operator()(const std::function<void()>& callback = nullptr) = 0;

    protected:
        [[nodiscard]] std::string get_identifier() const;
        size_t id = 0;

    private:
        void new_id();

        inline static size_t static_id_counter = 0;
    };

}
