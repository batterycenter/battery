
#include "battery/graphics/widgets/image.hpp"

namespace b::widgets {

    void image::operator()() {
        basePushStyle();

        ImVec2 _size = baseGetBBSize();          // If size is not set, use the size of the image in pixels
        if (_size.x == 0) _size.x = (float)src.getSize().x;
        if (_size.y == 0) _size.y = (float)src.getSize().y;     // TODO: Allow proportional scaling when only one is defined
        baseSetCursorPositionToMinBB();
        ImGui::Image(src, _size);
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();

        basePopStyle();
    }

}