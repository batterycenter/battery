
#include "battery/graphics/widgets/window.hpp"

namespace b::widgets {

    void window::operator()(const std::function<void()>& callback) {
        ImGui::Begin(get_identifier().c_str(), nullptr, flags);

        callback();

        ImGui::End();
    }

}