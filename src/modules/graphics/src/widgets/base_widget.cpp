
#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    base_widget::base_widget(py::object context, b::string name) : context(std::move(context)), name(std::move(name)) {
        createNewId();
    }

    b::string base_widget::baseGetIdentifier() const {
        return name + "##batteryui" + std::to_string(m_id);
    }

    static std::optional<double> getHorizontalPx(const unit_property& property) {
        std::optional<double> px;
        switch (property.unit()) {
            case b::Unit::UNITLESS: px = property.numeric(); break;
            case b::Unit::PIXEL: px = property.numeric(); break;
            case b::Unit::PERCENT: px = property.numeric() * (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x) / 100.0; break;
            case b::Unit::EM: px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    static std::optional<double> getVerticalPx(const unit_property& property) {
        std::optional<double> px;
        switch (property.unit()) {
            case b::Unit::UNITLESS: px = property.numeric(); break;
            case b::Unit::PIXEL: px = property.numeric(); break;
            case b::Unit::PERCENT: px = property.numeric() * (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y) / 100.0; break;
            case b::Unit::EM: px = property.numeric() * ImGui::GetFontSize(); break;
            default: break;
        }
        return px;
    }

    void base_widget::baseSetCursorPositionToMinBB() const {
        ImGui::SetCursorPos(baseGetBBMin());
    }

    std::pair<b::Vec2, b::Vec2> base_widget::baseGetBB() const {
        auto left_px = getHorizontalPx(left);
        auto right_px = getHorizontalPx(right);
        auto width_px = getHorizontalPx(width);
        auto top_px = getVerticalPx(top);
        auto bottom_px = getVerticalPx(bottom);
        auto height_px = getVerticalPx(height);

        double cursor_left = ImGui::GetCursorPosX();
        double cursor_width = 0;
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

        double cursor_top = ImGui::GetCursorPosY();
        double cursor_height = 0;
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

        return std::make_pair(b::Vec2(cursor_left, cursor_top), b::Vec2(cursor_left + cursor_width, cursor_top + cursor_height));
    }

    b::Vec2 base_widget::baseGetBBMin() const {
        return baseGetBB().first;
    }

    b::Vec2 base_widget::baseGetBBMax() const {
        return baseGetBB().second;
    }

    b::Vec2 base_widget::baseGetBBSize() const {
        return baseGetBB().second - baseGetBB().first;
    }

    void base_widget::basePushStyle() {
        style.push();
    }

    void base_widget::basePopStyle() {
        style.pop();
    }

    void base_widget::createNewId() {
        m_id = m_staticIdCounter;
        m_staticIdCounter++;
    }

}