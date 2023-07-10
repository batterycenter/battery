
#include "battery/graphics/widgets/panel.hpp"

namespace b::widgets {

    void panel::operator()(const std::function<void()>& callback) {
        basePushStyle();

        ImGui::SetNextWindowPos(baseGetBBMin());
        ImGui::SetNextWindowSize(baseGetBBSize());

        auto allFlags = rawFlags;
        if (flags.noMove) {
            allFlags |= ImGuiWindowFlags_NoMove;
        }
        if (flags.noResize) {
            allFlags |= ImGuiWindowFlags_NoResize;
        }
        if (flags.noCollapse) {
            allFlags |= ImGuiWindowFlags_NoCollapse;
        }
        if (flags.noTitleBar) {
            allFlags |= ImGuiWindowFlags_NoTitleBar;
        }
        if (flags.noScrollbar) {
            allFlags |= ImGuiWindowFlags_NoScrollbar;
        }
        if (flags.noBringToFrontOnFocus) {
            allFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        }
        if (flags.noNavFocus) {
            allFlags |= ImGuiWindowFlags_NoNavFocus;
        }
        if (flags.noBackground) {
            allFlags |= ImGuiWindowFlags_NoBackground;
        }

        if (!alwaysOpen) {
            ImGui::Begin(baseGetIdentifier().c_str(), &isOpen, allFlags);
        } else {
            ImGui::Begin(baseGetIdentifier().c_str(), nullptr, allFlags);
        }
        titlebarHovered = ImGui::IsItemHovered();
        actualPosition = ImGui::GetWindowPos();
        actualSize = ImGui::GetWindowSize();

        basePopStyle();

        if (callback) {
            childrenStyle.push();
            callback();
            childrenStyle.pop();
        }

        ImGui::End();
    }

}