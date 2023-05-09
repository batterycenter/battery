
#include "battery/graphics/widgets/window.hpp"

namespace b::widgets {

    void window::operator()(const std::function<void()>& callback) {
        style.push();

        if (position.has_value()) {
            ImGui::SetNextWindowPos(*position);
        }

        if (size.has_value()) {
            ImGui::SetNextWindowSize(*size);
        }

        ImGui::Begin(get_identifier().c_str(), nullptr, flags);
        callback();
        ImGui::End();

        actual_position = ImGui::GetWindowPos();
        actual_size = ImGui::GetWindowSize();

        style.pop();

    }

}