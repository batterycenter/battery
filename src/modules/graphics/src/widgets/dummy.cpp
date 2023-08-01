
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void vspace(double height) {
        ImGui::Dummy(b::Vec2(0.0, height));
    }

    void hspace(double width) {
        ImGui::Dummy(b::Vec2(width, 0.0));
    }

    void dummy(double width, double height) {
        ImGui::Dummy(b::Vec2(width, height));
    }

}