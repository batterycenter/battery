
#include "battery/graphics/styles.hpp"
#include "battery/graphics/color_hex.hpp"
#include "battery/graphics/property_stack.hpp"
#include "battery/core/resource_loader.hpp"
#include "resources/default_themes_json.hpp"
#include "magic_enum.hpp"

namespace b {

    struct themes {
        inline static std::mutex theme_mutex;
        inline static std::unordered_map<std::string, nlohmann::json> available_themes;
        inline static std::string current_theme = "default";
        inline static bool theme_reloading_requested = false;
    };

    void make_theme_available(const std::string& name, const nlohmann::json& data) {
        themes::available_themes[name] = data;
    }

    void make_default_themes_available() {
        static b::resource_loader loader(resources::default_themes_json, [&] (auto resource) {  // Must be & to capture themes::theme_mutex
            try {
                std::scoped_lock lock(themes::theme_mutex);
                auto style = nlohmann::json::parse(resource.as_string());
                for (auto &[name, theme]: style.items()) {
                    make_theme_available(name, theme);
                }
                themes::theme_reloading_requested = true;
            }
            catch (const std::exception& e) {
                b::log::error("Failed to make default_themes.json available: {}", e.what());
            }
        });
    }

    void load_theme(const std::string& name) {
        if (!themes::available_themes.contains(name))
            throw std::invalid_argument(fmt::format("Cannot load theme '{}': Theme does not exist", name));

        themes::current_theme = name;
        apply_theme(themes::available_themes[name]);
    }

    void update_themes() {
        std::scoped_lock lock(themes::theme_mutex);

        try {
            if (!themes::theme_reloading_requested)
                return;

            load_theme(themes::current_theme);
            themes::theme_reloading_requested = false;
        }
        catch (const std::exception& e) {
            b::log::error("Failed to load default_themes.json: {}", e.what());
        }
    }

    void apply_theme(const nlohmann::json& data) {
        property_stack::clear();

        for (auto& [key, value] : data.items()) {
            // If it is an ImGui StyleVar
            auto stylevar_enum = magic_enum::enum_cast<ImGuiStyleVar_>(key);
            if (stylevar_enum.has_value()) {
                switch(stylevar_enum.value()) {
                    case ImGuiStyleVar_Alpha: ImGui::GetStyle().Alpha = value; break;
                    case ImGuiStyleVar_DisabledAlpha: ImGui::GetStyle().DisabledAlpha = value; break;
                    case ImGuiStyleVar_WindowPadding: ImGui::GetStyle().WindowPadding = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_WindowRounding: ImGui::GetStyle().WindowRounding = value; break;
                    case ImGuiStyleVar_WindowBorderSize: ImGui::GetStyle().WindowBorderSize = value; break;
                    case ImGuiStyleVar_WindowMinSize: ImGui::GetStyle().WindowMinSize = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_WindowTitleAlign: ImGui::GetStyle().WindowTitleAlign = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_ChildRounding: ImGui::GetStyle().ChildRounding = value; break;
                    case ImGuiStyleVar_ChildBorderSize: ImGui::GetStyle().ChildBorderSize = value; break;
                    case ImGuiStyleVar_PopupRounding: ImGui::GetStyle().PopupRounding = value; break;
                    case ImGuiStyleVar_PopupBorderSize: ImGui::GetStyle().PopupBorderSize = value; break;
                    case ImGuiStyleVar_FramePadding: ImGui::GetStyle().FramePadding = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_FrameRounding: ImGui::GetStyle().FrameRounding = value; break;
                    case ImGuiStyleVar_FrameBorderSize: ImGui::GetStyle().FrameBorderSize = value; break;
                    case ImGuiStyleVar_ItemSpacing: ImGui::GetStyle().ItemSpacing = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_ItemInnerSpacing: ImGui::GetStyle().ItemInnerSpacing = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_IndentSpacing: ImGui::GetStyle().IndentSpacing = value; break;
                    case ImGuiStyleVar_CellPadding: ImGui::GetStyle().CellPadding = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_ScrollbarSize: ImGui::GetStyle().ScrollbarSize = value; break;
                    case ImGuiStyleVar_ScrollbarRounding: ImGui::GetStyle().ScrollbarRounding = value; break;
                    case ImGuiStyleVar_GrabMinSize: ImGui::GetStyle().GrabMinSize = value; break;
                    case ImGuiStyleVar_GrabRounding: ImGui::GetStyle().GrabRounding = value; break;
                    case ImGuiStyleVar_TabRounding: ImGui::GetStyle().TabRounding = value; break;
                    case ImGuiStyleVar_ButtonTextAlign: ImGui::GetStyle().ButtonTextAlign = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_SelectableTextAlign: ImGui::GetStyle().SelectableTextAlign = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_SeparatorTextBorderSize: ImGui::GetStyle().SeparatorTextBorderSize = value; break;
                    case ImGuiStyleVar_SeparatorTextAlign: ImGui::GetStyle().SeparatorTextAlign = ImVec2(value[0], value[1]); break;
                    case ImGuiStyleVar_SeparatorTextPadding: ImGui::GetStyle().SeparatorTextPadding = ImVec2(value[0], value[1]); break;
                    default: throw std::invalid_argument(fmt::format("Cannot apply theme: Unknown ImGuiStyleVar '{}'", key));
                }
                continue;
            }

            // If it is an ImGui Color
            auto color_enum = magic_enum::enum_cast<ImGuiCol_>(key);
            if (color_enum.has_value()) {
                auto colors = ImGui::GetStyle().Colors;
                colors[color_enum.value()] = color_hex(value).Value / 255;
                continue;
            }

            // Else we assume it is a custom property
            if (value.is_array()) {
                property_stack::push(key, b::property_pack({ value[0].get<std::string>(), value[1].get<std::string>() }));
            }
            else {
                property_stack::push(key, b::unit_property(value.get<std::string>()));
            }
        }
    }

}