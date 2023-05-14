
#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    base_widget::base_widget(py::object context, std::string name) : context(std::move(context)), name(std::move(name)) {
        new_id();
    }

    std::string base_widget::get_identifier() const {
        return name + "##batteryui" + std::to_string(id);
    }

    void base_widget::set_cursor_position() const {
        float fontsize = ImGui::GetFontSize();
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        std::optional<float> result_left;
        std::optional<float> result_top;

        // handle left
        switch (left.unit()) {
            case b::unit::UNITLESS: result_left = left.numeric(); break;
            case b::unit::PIXEL:    result_left = left.numeric(); break;
            case b::unit::PERCENT:  result_left = left.numeric() * (max.x - min.x) / 100.0f; break;
            case b::unit::EM:       result_left = left.numeric() * fontsize; break;
            default: break;
        }

        // handle top
        switch (top.unit()) {
            case b::unit::UNITLESS: result_top = top.numeric(); break;
            case b::unit::PIXEL:    result_top = top.numeric(); break;
            case b::unit::PERCENT:  result_top = top.numeric() * (max.y - min.y) / 100.0f; break;
            case b::unit::EM:       result_top = top.numeric() * fontsize; break;
            default: break;
        }

        if (result_left.has_value()) ImGui::SetCursorPosX(result_left.value());
        if (result_top.has_value())  ImGui::SetCursorPosY(result_top.value());
    }

    ImVec2 base_widget::desired_size() const {
        float fontsize = ImGui::GetFontSize();
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        std::optional<float> result_width;
        std::optional<float> result_height;

        // handle width
        switch (width.unit()) {
            case b::unit::UNITLESS: result_width = width.numeric(); break;
            case b::unit::PIXEL:    result_width = width.numeric(); break;
            case b::unit::PERCENT:  result_width = width.numeric() * (max.x - min.x) / 100.0f; break;
            case b::unit::EM:       result_width = width.numeric() * fontsize; break;
            default: break;
        }

        // handle height
        switch (height.unit()) {
            case b::unit::UNITLESS: result_height = height.numeric(); break;
            case b::unit::PIXEL:    result_height = height.numeric(); break;
            case b::unit::PERCENT:  result_height = height.numeric() * (max.y - min.y) / 100.0f; break;
            case b::unit::EM:       result_height = height.numeric() * fontsize; break;
            default: break;
        }

        return { result_width.value_or(0.0f), result_height.value_or(0.0f) };
    }

    void base_widget::new_id() {
        id = static_id_counter;
        static_id_counter++;
    }

}