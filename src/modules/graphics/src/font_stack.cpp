
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/graphics_constants.hpp"
#include "resources/fonts_inter_regular_ttf.hpp"
#include "resources/fonts_inter_medium_ttf.hpp"
#include "resources/fonts_inter_bold_ttf.hpp"
#include "resources/fonts_montserrat_regular_ttf.hpp"
#include "resources/fonts_montserrat_medium_ttf.hpp"
#include "resources/fonts_montserrat_bold_ttf.hpp"
#include "resources/fonts_roboto_regular_ttf.hpp"
#include "resources/fonts_roboto_medium_ttf.hpp"
#include "resources/fonts_roboto_bold_ttf.hpp"

namespace b {

    struct font_stack {
        inline static std::unordered_map<std::string, b::resource> available_fonts;
        inline static std::unordered_map<std::string, ImFont*> loaded_fonts;
        inline static bool locked = false;
    };

    void make_font_available(const std::string& font, const b::resource& ttf_file) {
        if (font_stack::locked)
            throw std::runtime_error("b::load_font() failed: Font loading operations cannot be performed during a render pass!");

        font_stack::available_fonts[font] = ttf_file;
    }

    ImFont* load_font(const std::string& identifier, const std::string& font, float size) {
        if (font_stack::locked)
            throw std::runtime_error("b::load_font() failed: Font loading operations cannot be performed during a render pass!");

        if (!font_stack::available_fonts.contains(font))
            throw std::invalid_argument(fmt::format("Cannot load font '{}': No such font was made available", font));

        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        auto ttf_resource = font_stack::available_fonts[font];
        auto new_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(ttf_resource.data(), static_cast<int>(ttf_resource.size()), size, &font_cfg);

        if (new_font == nullptr)
            throw std::runtime_error(fmt::format("Failed to load font '{}'", font));

        font_stack::loaded_fonts[identifier] = new_font;
        if (!ImGui::SFML::UpdateFontTexture())
            throw std::runtime_error(fmt::format("Failed to update font texture for font '{}'", font));

        return new_font;
    }

    void push_font(const std::string& font) {
        if (!font_stack::locked)
            throw std::runtime_error("b::push_font() failed: Font push/pop operations can only be performed during a render pass!");

        if (!font_stack::loaded_fonts.contains(font))
            throw std::invalid_argument(fmt::format("Cannot push font '{}': No such font was loaded", font));

        ImGui::PushFont(font_stack::loaded_fonts[font]);
    }

    void pop_font() {
        if (!font_stack::locked)
            throw std::runtime_error("b::pop_font() failed: Font push/pop operations can only be performed during a render pass!");

        ImGui::PopFont();
    }

    void lock_font_stack() {
        if (font_stack::locked)
            throw std::runtime_error("Cannot lock font stack: Font stack is already locked");
        font_stack::locked = true;
    }

    void unlock_font_stack() {
        if (!font_stack::locked)
            throw std::runtime_error("Cannot unlock font stack: Font stack is not locked");
        font_stack::locked = false;
    }

    std::vector<std::string> get_available_fonts() {
        std::vector<std::string> fonts;
        for (auto& [font, _] : font_stack::available_fonts) {
            fonts.push_back(font);
        }
        return fonts;
    }

    void load_default_fonts() {
        make_font_available("inter-regular", resources::fonts_inter_regular_ttf);
        make_font_available("inter-medium", resources::fonts_inter_medium_ttf);
        make_font_available("inter-bold", resources::fonts_inter_bold_ttf);
        make_font_available("roboto-regular", resources::fonts_roboto_regular_ttf);
        make_font_available("roboto-medium", resources::fonts_roboto_medium_ttf);
        make_font_available("roboto-bold", resources::fonts_roboto_bold_ttf);
        make_font_available("montserrat-regular", resources::fonts_montserrat_regular_ttf);
        make_font_available("montserrat-medium", resources::fonts_montserrat_medium_ttf);
        make_font_available("montserrat-bold", resources::fonts_montserrat_bold_ttf);

        auto [default_font, default_font_size] = b::graphics_constants::battery_default_font();
        b::load_font("default", default_font, default_font_size);
    }

}