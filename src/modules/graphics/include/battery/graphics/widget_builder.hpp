#pragma once

#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/widget_style.hpp"

namespace b::widgets {

    class widget_builder {
    public:
        class numeric_rule {
        public:
            numeric_rule(const char* tag) : tag(tag) {}
            ~numeric_rule();

            numeric_rule& add_case(enum b::unit unit, const std::function<float(float)>& transformer);
            std::optional<float> get_result();
            float get_result(float default_value);
            void push(ImGuiStyleVar_ style_var);

            numeric_rule& operator=(const numeric_rule&) = delete;
            numeric_rule(const numeric_rule&) = delete;

            numeric_rule& operator=(numeric_rule&& other) {
                this->tag = other.tag;
                this->pushed = other.pushed;
                this->cases = std::move(other.cases);
                other.pushed = 0;
                return *this;
            }
            numeric_rule(numeric_rule&& other) {
                operator=(std::move(other));
            };

        private:
            const char* tag;
            int pushed = 0;
            std::vector<std::pair<enum b::unit, std::function<float(float)>>> cases;
        };

        class vec2_rule {
        public:
            vec2_rule(const char* tag) : tag(tag) {}
            ~vec2_rule();

            vec2_rule& add_case_x(enum b::unit unit, const std::function<float(float)>& transformer);
            vec2_rule& add_case_y(enum b::unit unit, const std::function<float(float)>& transformer);
            std::optional<ImVec2> get_result();
            ImVec2 get_result(ImVec2 default_value);
            void push(ImGuiStyleVar_ style_var);

            vec2_rule& operator=(const vec2_rule&) = delete;
            vec2_rule(const vec2_rule&) = delete;

            vec2_rule& operator=(vec2_rule&& other) {
                this->tag = other.tag;
                this->pushed = other.pushed;
                this->cases_x = std::move(other.cases_x);
                this->cases_y = std::move(other.cases_y);
                other.pushed = 0;
                return *this;
            }
            vec2_rule(vec2_rule&& other) {
                operator=(std::move(other));
            };

        private:
            const char* tag;
            int pushed = 0;
            std::vector<std::pair<enum b::unit, std::function<float(float)>>> cases_x;
            std::vector<std::pair<enum b::unit, std::function<float(float)>>> cases_y;
        };

        class color_rule {
        public:
            color_rule(const char* tag) : tag(tag) {}
            ~color_rule();

            std::optional<ImVec4> get_result();
            ImVec4 get_result(ImVec4 default_value);
            void push(ImGuiCol_ style_var);

            color_rule& operator=(const color_rule&) = delete;
            color_rule(const color_rule&) = delete;

            color_rule& operator=(color_rule&& other) {
                this->tag = other.tag;
                this->pushed = other.pushed;
                this->cases = std::move(other.cases);
                other.pushed = 0;
                return *this;
            }
            color_rule(color_rule&& other) {
                operator=(std::move(other));
            };

        private:
            const char* tag;
            int pushed = 0;
            std::vector<std::pair<enum b::unit, std::function<ImVec4(ImVec4)>>> cases;
        };

    public:
        widget_builder(widget_style& style);
        ~widget_builder();

        void pop();

        numeric_rule& add_numeric_rule(const char* tag);
        vec2_rule& add_vec2_rule(const char* tag);
        color_rule& add_color_rule(const char* tag);

    private:
        widget_style& style;
        bool released = false;
        std::vector<numeric_rule> numeric_rules;
        std::vector<vec2_rule> vec2_rules;
        std::vector<color_rule> color_rules;
    };

}