
#include "battery/graphics/widgets/dummy.hpp"
#include "battery/graphics/sfml.hpp"

namespace b::widgets {

    void vspace(float height) {
        ImGui::Dummy(ImVec2(0.0f, height));
    }

    void hspace(float width) {
        ImGui::Dummy(ImVec2(width, 0.0f));
    }

    void dummy(float width, float height) {
        ImGui::Dummy(ImVec2(width, height));
    }

}