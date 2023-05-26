
#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    base_widget::base_widget(py::object context, b::string name) : context(std::move(context)), name(std::move(name)) {
        new_id();
    }

    b::string base_widget::base_get_identifier() const {
        return name + "##batteryui" + std::to_string(id);
    }

    static std::optional<float> get_horizontal_px(const unit_property& property) {
        std::optional<float> px;
        switch (property.unit()) {
            case b::unit::UNITLESS: px = property.numeric(); break;
            case b::unit::PIXEL:    px = property.numeric(); break;
            case b::unit::PERCENT:  px = property.numeric() * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 100.0f; break;
            case b::unit::EM:       px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    static std::optional<float> get_vertical_px(const unit_property& property) {
        std::optional<float> px;
        switch (property.unit()) {
            case b::unit::UNITLESS: px = property.numeric(); break;
            case b::unit::PIXEL:    px = property.numeric(); break;
            case b::unit::PERCENT:  px = property.numeric() * (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) / 100.0f; break;
            case b::unit::EM:       px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    void base_widget::base_set_cursor_position_to_min_bb() const {
        ImGui::SetCursorPos(base_get_bb_min());
    }

    std::pair<ImVec2, ImVec2> base_widget::base_get_bb() const {
        auto left_px = get_horizontal_px(left);
        auto right_px = get_horizontal_px(right);
        auto width_px = get_horizontal_px(width);
        auto top_px = get_vertical_px(top);
        auto bottom_px = get_vertical_px(bottom);
        auto height_px = get_vertical_px(height);

        float cursor_left = ImGui::GetCursorPosX();
        float cursor_width = 0;
        if (width_px.has_value() && right_px.has_value()) {
            cursor_left = ImGui::GetWindowContentRegionMax().x - right_px.value() - width_px.value();
            cursor_width = width_px.value();
        }
        else if (left_px.has_value() && right_px.has_value()) {
            cursor_left = left_px.value();
            cursor_width = ImGui::GetWindowContentRegionMax().x - right_px.value() - left_px.value();
        }
        else if (left_px.has_value() && width_px.has_value()) {
            cursor_left = left_px.value();
            cursor_width = width_px.value();
        }
        else if (right_px.has_value()) {
            cursor_width = ImGui::GetWindowContentRegionMax().x - right_px.value() - cursor_left;
        }
        else if (left_px.has_value()) {
            cursor_left = left_px.value();
        }
        else if (width_px.has_value()) {
            cursor_width = width_px.value();
        }

        float cursor_top = ImGui::GetCursorPosY();
        float cursor_height = 0;
        if (height_px.has_value() && bottom_px.has_value()) {
            cursor_top = ImGui::GetWindowContentRegionMax().y - bottom_px.value() - height_px.value();
            cursor_height = height_px.value();
        }
        else if (top_px.has_value() && bottom_px.has_value()) {
            cursor_top = top_px.value();
            cursor_height = ImGui::GetWindowContentRegionMax().y - bottom_px.value() - top_px.value();
        }
        else if (top_px.has_value() && height_px.has_value()) {
            cursor_top = top_px.value();
            cursor_height = height_px.value();
        }
        else if (bottom_px.has_value()) {
            cursor_height = ImGui::GetWindowContentRegionMax().y - bottom_px.value() - cursor_top;
        }
        else if (top_px.has_value()) {
            cursor_top = top_px.value();
        }
        else if (height_px.has_value()) {
            cursor_height = height_px.value();
        }

        return std::make_pair(ImVec2(cursor_left, cursor_top), ImVec2(cursor_left + cursor_width, cursor_top + cursor_height));
    }

    ImVec2 base_widget::base_get_bb_min() const {
        return base_get_bb().first;
    }

    ImVec2 base_widget::base_get_bb_max() const {
        return base_get_bb().second;
    }

    ImVec2 base_widget::base_get_bb_size() const {
        return base_get_bb().second - base_get_bb().first;
    }

    void base_widget::base_push_style() {
        style.push();
    }

    void base_widget::base_pop_style() {
        style.pop();
    }

    void base_widget::new_id() {
        id = static_id_counter;
        static_id_counter++;
    }

}