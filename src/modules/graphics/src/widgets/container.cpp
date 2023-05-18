
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    void container::operator()() {
        this->operator()({});
    }

    void container::operator()(const std::function<void()>& callback) {
        base_push_style();

        base_set_cursor_position_to_min_bb();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::BeginChild(base_get_identifier().c_str(), base_get_bb_size(), native_window_border, flags);
        ImGui::PopStyleVar();

        base_pop_style();

        if (callback) {
            children_style.push();
            callback();
            children_style.pop();
        }

        ImGui::EndChild();
        actual_position = ImGui::GetItemRectMin();
        actual_size = ImGui::GetItemRectSize();
    }

}