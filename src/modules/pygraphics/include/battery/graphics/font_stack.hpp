#pragma once

#include "battery/core/resource.hpp"
#include "battery/graphics/sfml.hpp"

namespace b {

    void MakeFontAvailable(const b::string& font, const b::Resource& ttf_file);

    ImFont* LoadFont(const b::string& identifier, const b::string& font, float size);
    void PushFont(const b::string& font);
    void PopFont();

    void LockFontStack();
    void UnlockFontStack();

    std::vector<b::string> GetAvailableFonts();
    void LoadDefaultFonts();

}
