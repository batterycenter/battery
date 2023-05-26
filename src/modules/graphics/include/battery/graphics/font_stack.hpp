#pragma once

#include "battery/core/resource.hpp"
#include "battery/graphics/sfml.hpp"

namespace b {

    void make_font_available(const b::string& font, const b::resource& ttf_file);

    ImFont* load_font(const b::string& identifier, const b::string& font, float size);
    void push_font(const b::string& font);
    void pop_font();

    void lock_font_stack();
    void unlock_font_stack();

    std::vector<b::string> get_available_fonts();
    void load_default_fonts();

}
