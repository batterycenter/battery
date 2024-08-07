
#include "battery/graphics/widgets/button.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void button::operator()() {
        basePushStyle();

        baseSetCursorPositionToMinBB();
        if (custom_implementation) {
            std::tie(clicked, hovered, held) = custom_implementation();
        }
        else {
            clicked = ImGui::Button(baseGetIdentifier().c_str(), baseGetBBSize());
        }

        held = ImGui::IsItemActive();
        hovered = ImGui::IsItemHovered();
        actualPosition = ImGui::GetItemRectMin();
        actualSize = ImGui::GetItemRectSize();

        basePopStyle();
    }

}