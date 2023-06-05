
#include "battery/graphics/styles.hpp"
#include "battery/core/resource_loader.hpp"
#include "battery/graphics/color_hex.hpp"
#include "battery/graphics/property_stack.hpp"
#include "resources/default_themes_json.hpp"
#include "magic_enum.hpp"

namespace b {

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace Themes {
        inline static std::mutex& ThemeMutex() {
            static std::mutex mutex;
            return mutex;
        }

        inline static std::unordered_map<b::string, nlohmann::json>& AvailableThemes() {
            static std::unordered_map<b::string, nlohmann::json> themes;
            return themes;
        }

        inline static b::string& CurrentTheme() {
            static b::string themeString = "default";
            return themeString;
        }

        inline static bool& ThemeReloadingRequested() {
            static bool isRequested = false;
            return isRequested;
        }
    } // namespace Themes

    void make_theme_available(const b::string& name, const nlohmann::json& data) {
        Themes::AvailableThemes()[name] = data;
    }

    void make_default_themes_available() {
        static b::ResourceLoader const loader(Resources::DEFAULT_THEMES_JSON, [&] (auto resource) {  // Must be & to capture themes::theme_mutex
            try {
                const std::scoped_lock lock(Themes::ThemeMutex());
                auto style = nlohmann::json::parse(resource.string());
                for (auto &[name, theme]: style.items()) {
                    make_theme_available(name, theme);
                }
                Themes::ThemeReloadingRequested() = true;
            }
            catch (const std::exception& e) {
                b::log::error("Failed to make default_themes.json available: {}", e.what());
            }
        });
    }

    void load_theme(const b::string& name) {
        if (!Themes::AvailableThemes().contains(name)) {
            throw std::invalid_argument(b::format("Cannot load theme '{}': Theme does not exist", name));
        }

        Themes::CurrentTheme() = name;
        apply_theme(Themes::AvailableThemes()[name]);
    }

    void update_themes() {
        auto lock = std::scoped_lock(Themes::ThemeMutex());

        try {
            if (!Themes::ThemeReloadingRequested()) {
                return;
            }

            load_theme(Themes::CurrentTheme());
            Themes::ThemeReloadingRequested() = false;
        }
        catch (const std::exception& e) {
            b::log::error("Failed to load default_themes.json: {}", e.what());
        }
    }

    void apply_theme(const nlohmann::json& data) {
        property_stack::clear();

        for (const auto& [key, value] : data.items()) {
            // If it is an ImGui StyleVar
            auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(key);
            if (stylevarEnum.has_value()) {
                switch(stylevarEnum.value()) {
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
                    default: throw std::invalid_argument(b::format("Cannot apply theme: Unknown ImGuiStyleVar '{}'", key));
                }
                continue;
            }

            // If it is an ImGui Color
            auto colorEnum = magic_enum::enum_cast<ImGuiCol_>(key);
            if (colorEnum.has_value()) {
                auto arrayIndex = colorEnum.value();
                if (arrayIndex < ImGuiCol_COUNT) {
                    ImGui::GetStyle().Colors[arrayIndex] = color_hex(std::string(value)).Value / 255;   // NOLINT
                }
                else {
                    throw std::invalid_argument(b::format("Cannot apply theme: Unknown ImGuiCol '{}'", key));
                }
                continue;
            }

            // Else we assume it is a custom property
            if (value.is_array()) {
                property_stack::push(key, b::property_pack({ value[0].get<b::string>(), value[1].get<b::string>() }));
            }
            else {
                property_stack::push(key, b::unit_property(value.get<b::string>()));
            }
        }
    }

} // namespace b