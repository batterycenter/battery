
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    void checkbox::operator()() {
        set_cursor_position();
        ImGui::Checkbox(get_identifier().c_str(), &state);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
    }

}