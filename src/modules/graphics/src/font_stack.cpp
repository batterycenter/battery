
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/constants.hpp"
#include "resources/FontsInterBold_ttf.hpp"
#include "resources/FontsInterMedium_ttf.hpp"
#include "resources/FontsInterRegular_ttf.hpp"
#include "resources/FontsMontserratBold_ttf.hpp"
#include "resources/FontsMontserratMedium_ttf.hpp"
#include "resources/FontsMontserratRegular_ttf.hpp"
#include "resources/FontsRobotoBold_ttf.hpp"
#include "resources/FontsRobotoMedium_ttf.hpp"
#include "resources/FontsRobotoRegular_ttf.hpp"

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
        MakeFontAvailable("inter-regular", Resources::FontsInterRegular_ttf);       // TODO: Make default fonts optional
        MakeFontAvailable("inter-medium", Resources::FontsInterMedium_ttf);
        MakeFontAvailable("inter-bold", Resources::FontsInterBold_ttf);
        MakeFontAvailable("roboto-regular", Resources::FontsRobotoRegular_ttf);
        MakeFontAvailable("roboto-medium", Resources::FontsRobotoMedium_ttf);
        MakeFontAvailable("roboto-bold", Resources::FontsRobotoBold_ttf);
        MakeFontAvailable("montserrat-regular", Resources::FontsMontserratRegular_ttf);
        MakeFontAvailable("montserrat-medium", Resources::FontsMontserratMedium_ttf);
        MakeFontAvailable("montserrat-bold", Resources::FontsMontserratBold_ttf);

        auto [default_font, default_font_size] = b::Constants::DefaultFont();
        b::LoadFont("default", default_font, default_font_size);
    }

} // namespace b