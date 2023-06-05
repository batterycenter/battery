
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

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace FontStack {
        inline static std::unordered_map<b::string, b::Resource>& AvailableFonts() {
            static std::unordered_map<b::string, b::Resource> availableFontsMap;
            return availableFontsMap;
        }

        inline static std::unordered_map<b::string, ImFont*>& LoadedFonts() {
            static std::unordered_map<b::string, ImFont*> loadedFontsMap;
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

    void MakeFontAvailable(const b::string& font, const b::Resource &ttf_file) {
        if (FontStack::Locked()) {
            throw std::runtime_error("b::LoadFont() failed: Font loading operations cannot be performed during a render pass!");
        }

        FontStack::AvailableFonts()[font] = ttf_file;
    }

    ImFont* LoadFont(const b::string& identifier, const b::string& font, float size) {
        if (FontStack::Locked()) {
            throw std::runtime_error("b::LoadFont() failed: Font loading operations cannot be performed during a render pass!");
        }

        if (!FontStack::AvailableFonts().contains(font)) {
            throw std::invalid_argument(b::format("Cannot load font '{}': No such font was made available", font));
        }

        ImFontConfig fontCfg;
        fontCfg.FontDataOwnedByAtlas = false;
        auto ttfResource = FontStack::AvailableFonts()[font];
        auto *newFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(std::bit_cast<void*>(ttfResource.data()), static_cast<int>(ttfResource.size()), size, &fontCfg);

        if (newFont == nullptr) {
            throw std::runtime_error(b::format("Failed to load font '{}'", font));
        }

        FontStack::LoadedFonts()[identifier] = newFont;
        if (!ImGui::SFML::UpdateFontTexture()) {
            throw std::runtime_error(b::format("Failed to update font texture for font '{}'", font));
        }

        return newFont;
    }

    void PushFont(const b::string& font) {
        if (!FontStack::Locked()) {
            throw std::runtime_error("b::PushFont() failed: Font push/pop operations can only be performed during a render pass!");
        }

        if (!FontStack::LoadedFonts().contains(font)) {
            throw std::invalid_argument(b::format("Cannot push font '{}': No such font was loaded", font));
        }

        ImGui::PushFont(FontStack::LoadedFonts()[font]);
    }

    void PopFont() {
        if (!FontStack::Locked()) {
            throw std::runtime_error("b::PopFont() failed: Font push/pop operations can only be performed during a render pass!");
        }

        ImGui::PopFont();
    }

    void LockFontStack() {
        if (FontStack::Locked()) {
            throw std::runtime_error("Cannot lock font stack: Font stack is already locked");
        }
        FontStack::Locked(true);
    }

    void UnlockFontStack() {
        if (!FontStack::Locked()) {
            throw std::runtime_error("Cannot unlock font stack: Font stack is not locked");
        }
        FontStack::Locked(false);
    }

    std::vector<b::string> GetAvailableFonts() {
        std::vector<b::string> fonts;
        fonts.reserve(FontStack::AvailableFonts().size());
        for (auto& [font, _] : FontStack::AvailableFonts()) {
            fonts.push_back(font);
        }
        return fonts;
    }

    void LoadDefaultFonts() {
        MakeFontAvailable("inter-regular", Resources::FONTS_INTER_REGULAR_TTF);       // TODO: Make default fonts optional
        MakeFontAvailable("inter-medium", Resources::FONTS_INTER_MEDIUM_TTF);
        MakeFontAvailable("inter-bold", Resources::FONTS_INTER_BOLD_TTF);
        MakeFontAvailable("roboto-regular", Resources::FONTS_ROBOTO_REGULAR_TTF);
        MakeFontAvailable("roboto-medium", Resources::FONTS_ROBOTO_MEDIUM_TTF);
        MakeFontAvailable("roboto-bold", Resources::FONTS_ROBOTO_BOLD_TTF);
        MakeFontAvailable("montserrat-regular", Resources::FONTS_MONTSERRAT_REGULAR_TTF);
        MakeFontAvailable("montserrat-medium", Resources::FONTS_MONTSERRAT_MEDIUM_TTF);
        MakeFontAvailable("montserrat-bold", Resources::FONTS_MONTSERRAT_BOLD_TTF);

        auto [default_font, default_font_size] = b::graphics_constants::battery_default_font();
        b::LoadFont("default", default_font, default_font_size);
    }

} // namespace b