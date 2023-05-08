#pragma once

#include "battery/core/resource.hpp"
#include "battery/graphics/sfml.hpp"

namespace b {

    void make_font_available(const std::string& font, const b::resource& ttf_file);

    ImFont* load_font(const std::string& identifier, const std::string& font, float size);
    void push_font(const std::string& font);
    void pop_font();

    std::vector<std::string> get_available_fonts();
    void load_default_fonts();

}
