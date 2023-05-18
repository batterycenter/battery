
#include "battery/graphics/widgets/image.hpp"

namespace b::widgets {

    void image::operator()() {
        base_push_style();

        ImVec2 _size = base_get_bb_size();          // If size is not set, use the size of the image in pixels
        if (_size.x == 0) _size.x = (float)src.getSize().x;
        if (_size.y == 0) _size.y = (float)src.getSize().y;     // TODO: Allow proportional scaling when only one is defined
        base_set_cursor_position_to_min_bb();
        ImGui::Image(src, _size);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        base_pop_style();
    }

}