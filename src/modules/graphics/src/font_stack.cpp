
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/graphics_constants.hpp"
#include "resources/fonts_inter_bold_ttf.hpp"
#include "resources/fonts_inter_medium_ttf.hpp"
#include "resources/fonts_inter_regular_ttf.hpp"
#include "resources/fonts_montserrat_bold_ttf.hpp"
#include "resources/fonts_montserrat_medium_ttf.hpp"
#include "resources/fonts_montserrat_regular_ttf.hpp"
#include "resources/fonts_roboto_bold_ttf.hpp"
#include "resources/fonts_roboto_medium_ttf.hpp"
#include "resources/fonts_roboto_regular_ttf.hpp"

namespace b {

    namespace FontStack {
        inline static std::unordered_map<b::string, b::resource> AvailableFonts() {
            static std::unordered_map<b::string, b::resource> const availableFontsMap;
            return availableFontsMap;
        }

        inline static std::unordered_map<b::string, ImFont*> LoadedFonts() {
            static std::unordered_map<b::string, ImFont*> const loadedFontsMap;
            return loadedFontsMap;
        }

        inline static bool Locked(std::optional<bool> locked = {}) {
            static bool isLocked = false;
            if (locked.has_value()) {
                isLocked = locked.value();
            }
            return isLocked;
        }
    } // namespace FontStack

    void make_font_available(const b::string& font, const b::resource& ttf_file) {
        if (FontStack::Locked()) {
            throw std::runtime_error("b::load_font() failed: Font loading operations cannot be performed during a render pass!");
        }

        FontStack::AvailableFonts()[font] = ttf_file;
    }

    ImFont* load_font(const b::string& identifier, const b::string& font, float size) {
        if (FontStack::Locked()) {
            throw std::runtime_error("b::load_font() failed: Font loading operations cannot be performed during a render pass!");
        }

        if (!FontStack::AvailableFonts().contains(font)) {
            throw std::invalid_argument(b::format("Cannot load font '{}': No such font was made available", font));
        }

        ImFontConfig fontCfg;
        fontCfg.FontDataOwnedByAtlas = false;
        auto ttfResource = FontStack::AvailableFonts()[font];
        auto *newFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(ttfResource.data(), static_cast<int>(ttfResource.size()), size, &fontCfg);

        if (newFont == nullptr) {
            throw std::runtime_error(b::format("Failed to load font '{}'", font));
        }

        FontStack::LoadedFonts()[identifier] = newFont;
        if (!ImGui::SFML::UpdateFontTexture()) {
            throw std::runtime_error(b::format("Failed to update font texture for font '{}'", font));
        }

        return newFont;
    }

    void push_font(const b::string& font) {
        if (!FontStack::Locked()) {
            throw std::runtime_error("b::push_font() failed: Font push/pop operations can only be performed during a render pass!");
        }

        if (!FontStack::LoadedFonts().contains(font)) {
            throw std::invalid_argument(b::format("Cannot push font '{}': No such font was loaded", font));
        }

        ImGui::PushFont(FontStack::LoadedFonts()[font]);
    }

    void pop_font() {
        if (!FontStack::Locked()) {
            throw std::runtime_error("b::pop_font() failed: Font push/pop operations can only be performed during a render pass!");
        }

        ImGui::PopFont();
    }

    void lock_font_stack() {
        if (FontStack::Locked()) {
            throw std::runtime_error("Cannot lock font stack: Font stack is already locked");
        }
        FontStack::Locked(true);
    }

    void unlock_font_stack() {
        if (!FontStack::Locked()) {
            throw std::runtime_error("Cannot unlock font stack: Font stack is not locked");
        }
        FontStack::Locked(false);
    }

    std::vector<b::string> get_available_fonts() {
        std::vector<b::string> fonts;
        fonts.reserve(FontStack::AvailableFonts().size());
        for (auto& [font, _] : FontStack::AvailableFonts()) {
            fonts.push_back(font);
        }
        return fonts;
    }

    void load_default_fonts() {
        make_font_available("inter-regular", resources::FONTS_INTER_REGULAR_TTF);       // TODO: Make default fonts optional
        make_font_available("inter-medium", resources::FONTS_INTER_MEDIUM_TTF);
        make_font_available("inter-bold", resources::FONTS_INTER_BOLD_TTF);
        make_font_available("roboto-regular", resources::FONTS_ROBOTO_REGULAR_TTF);
        make_font_available("roboto-medium", resources::FONTS_ROBOTO_MEDIUM_TTF);
        make_font_available("roboto-bold", resources::FONTS_ROBOTO_BOLD_TTF);
        make_font_available("montserrat-regular", resources::FONTS_MONTSERRAT_REGULAR_TTF);
        make_font_available("montserrat-medium", resources::FONTS_MONTSERRAT_MEDIUM_TTF);
        make_font_available("montserrat-bold", resources::FONTS_MONTSERRAT_BOLD_TTF);

        auto [default_font, default_font_size] = b::graphics_constants::battery_default_font();
        b::load_font("default", default_font, default_font_size);
    }

} // namespace b