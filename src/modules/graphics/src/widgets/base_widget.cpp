
#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    base_widget::base_widget(py::object context, std::string name) : context(std::move(context)), name(std::move(name)) {
        new_id();
    }

    std::string base_widget::base_get_identifier() const {
        return name + "##batteryui" + std::to_string(id);
    }

    static float get_left_px(const unit_property& property) {
        float px = ImGui::GetCursorPosX();
        switch (property.unit()) {
            case b::unit::UNITLESS: px = property.numeric(); break;
            case b::unit::PIXEL:    px = property.numeric(); break;
            case b::unit::PERCENT:  px = property.numeric() * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 100.0f; break;
            case b::unit::EM:       px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    static float get_top_px(const unit_property& property) {
        float px = ImGui::GetCursorPosY();
        switch (property.unit()) {
            case b::unit::UNITLESS: px = property.numeric(); break;
            case b::unit::PIXEL:    px = property.numeric(); break;
            case b::unit::PERCENT:  px = property.numeric() * (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) / 100.0f; break;
            case b::unit::EM:       px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    static float get_width_px(float left_px, const unit_property& width, const unit_property& right) {
        if (width.unit() != b::unit::NONE) {    // Width is defined: use it
            float px = 0;
            switch (width.unit()) {
                case b::unit::UNITLESS: px = width.numeric(); break;
                case b::unit::PIXEL:    px = width.numeric(); break;
                case b::unit::PERCENT:  px = width.numeric() * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 100.0f; break;
                case b::unit::EM:       px = width.numeric() * ImGui::GetFontSize(); break;
                default: break;
            }
            return px;
        }
        else {                                      // Otherwise, right is used
            float _px_from_right = NAN;
            switch (right.unit()) {
                case b::unit::UNITLESS: _px_from_right = right.numeric(); break;
                case b::unit::PIXEL:    _px_from_right = right.numeric(); break;
                case b::unit::PERCENT:  _px_from_right = right.numeric() * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 100.0f; break;
                case b::unit::EM:       _px_from_right = right.numeric() * ImGui::GetFontSize(); break;
                default: break;
            }
            return !isnan(_px_from_right) ? ImGui::GetWindowContentRegionMax().x - _px_from_right - left_px : 0;
        }
    }

    static float get_height_px(float top_px, const unit_property& height, const unit_property& bottom) {
        if (height.unit() != b::unit::NONE) {    // Height is defined: use it
            float px = 0;
            switch (height.unit()) {
                case b::unit::UNITLESS: px = height.numeric(); break;
                case b::unit::PIXEL:    px = height.numeric(); break;
                case b::unit::PERCENT:  px = height.numeric() * (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) / 100.0f; break;
                case b::unit::EM:       px = height.numeric() * ImGui::GetFontSize(); break;
                default: break;
            }
            return px;
        }
        else {                                      // Otherwise, bottom is used
            float _px_from_bottom = NAN;
            switch (bottom.unit()) {
                case b::unit::UNITLESS: _px_from_bottom = bottom.numeric(); break;
                case b::unit::PIXEL:    _px_from_bottom = bottom.numeric(); break;
                case b::unit::PERCENT:  _px_from_bottom = bottom.numeric() * (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) / 100.0f; break;
                case b::unit::EM:       _px_from_bottom = bottom.numeric() * ImGui::GetFontSize(); break;
                default: break;
            }
            return !isnan(_px_from_bottom) ? ImGui::GetWindowContentRegionMax().y - _px_from_bottom - top_px : 0;
        }
    }

    void base_widget::base_set_cursor_position_to_min_bb() const {
        ImGui::SetCursorPos(base_get_bb_min());
    }

    ImVec2 base_widget::base_get_bb_min() const {
        return ImVec2(get_left_px(left), get_top_px(top));
    }

    ImVec2 base_widget::base_get_bb_max() const {
        return base_get_bb_min() + base_get_bb_size();
    }

    ImVec2 base_widget::base_get_bb_size() const {
        return ImVec2(get_width_px(get_left_px(left), width, right), get_height_px(get_top_px(top), height, bottom));
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