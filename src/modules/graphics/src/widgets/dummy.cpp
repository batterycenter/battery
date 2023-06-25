
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void vspace(double height) {
        ImGui::Dummy(b::vec2(0.0, height));
    }

    void hspace(double width) {
        ImGui::Dummy(b::vec2(width, 0.0));
    }

    void dummy(double width, double height) {
        ImGui::Dummy(b::vec2(width, height));
    }

}