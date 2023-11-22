
#include "battery/graphics/styles.hpp"
#include "battery/resource_loader.hpp"
#include "battery/graphics/color_hex.hpp"
#include "battery/graphics/property_stack.hpp"
#include "resources/DefaultThemesJson.hpp"
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
        static b::ResourceLoader const loader(Resources::DefaultThemesJson, [&] (auto resource) {  // Must be & to capture themes::theme_mutex
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

    }

} // namespace b