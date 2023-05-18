
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    void container::operator()() {
        this->operator()({});
    }

    void container::operator()(const std::function<void()>& callback) {

        set_cursor_position();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::BeginChild(get_identifier().c_str(), desired_size(), native_window_border, flags);
        ImGui::PopStyleVar();

        if (callback) {
            callback();
        }

        ImGui::EndChild();
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
    }

}