
#include "battery/imguilua.hpp"
#include "battery/log.hpp"
#include "battery/extern/magic_enum.hpp"

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#define ADD_STATIC_FUNCTION(name) \
        addStaticFunction(#name, &ImGuiLua::name)

namespace b {

    bool ImGuiLua::Begin(const std::string &name) {
        return ImGui::Begin(name.data());
    }

    void ImGuiLua::End() {
        ImGui::End();
    }

    void ImGuiLua::ShowDemoWindow() {
        ImGui::ShowDemoWindow();
    }

    void ImGuiLua::Text(const std::string& text) {
        ImGui::Text("%s", text.data());
    }

    bool ImGuiLua::Button(const std::string& text, const ImVec2& size) {
        return ImGui::Button(text.data(), size);
    }


    void ImGuiLua::ApplyStyleColorPermanent(const std::string& enumValue, const ImColor& color) {
        auto colorEnum = magic_enum::enum_cast<ImGuiCol_>(enumValue);
        if (!colorEnum) {
            throw std::invalid_argument(
                    b::format("ImGuiLua::ApplyStyleColorPermanent: Unknown ImGuiCol '{}'", enumValue));
        }

        if (*colorEnum < ImGuiCol_COUNT) {
            ImGui::GetStyle().Colors[*colorEnum] = color;   // NOLINT
        }
        else {
            throw std::invalid_argument(
                    b::format("ImGuiLua::ApplyStyleColorPermanent: Unknown ImGuiCol '{}'", enumValue));
        }
    }

    void ImGuiLua::ApplyStyleVec2Permanent(const std::string& enumValue, const ImVec2& value) {
        auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
        if (!stylevarEnum) {
            throw std::invalid_argument(
                    b::format("ImGuiLua::ApplyStyleVec2Permanent: Unknown ImGuiStyleVar '{}'", enumValue));
        }

        switch(stylevarEnum.value()) {
//            case ImGuiStyleVar_Alpha: ImGui::GetStyle().Alpha = value; break;
//            case ImGuiStyleVar_DisabledAlpha: ImGui::GetStyle().DisabledAlpha = value; break;
            case ImGuiStyleVar_WindowPadding: ImGui::GetStyle().WindowPadding = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_WindowRounding: ImGui::GetStyle().WindowRounding = value; break;
//            case ImGuiStyleVar_WindowBorderSize: ImGui::GetStyle().WindowBorderSize = value; break;
            case ImGuiStyleVar_WindowMinSize: ImGui::GetStyle().WindowMinSize = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_WindowTitleAlign: ImGui::GetStyle().WindowTitleAlign = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_ChildRounding: ImGui::GetStyle().ChildRounding = value; break;
//            case ImGuiStyleVar_ChildBorderSize: ImGui::GetStyle().ChildBorderSize = value; break;
//            case ImGuiStyleVar_PopupRounding: ImGui::GetStyle().PopupRounding = value; break;
//            case ImGuiStyleVar_PopupBorderSize: ImGui::GetStyle().PopupBorderSize = value; break;
            case ImGuiStyleVar_FramePadding: ImGui::GetStyle().FramePadding = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_FrameRounding: ImGui::GetStyle().FrameRounding = value; break;
//            case ImGuiStyleVar_FrameBorderSize: ImGui::GetStyle().FrameBorderSize = value; break;
            case ImGuiStyleVar_ItemSpacing: ImGui::GetStyle().ItemSpacing = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_ItemInnerSpacing: ImGui::GetStyle().ItemInnerSpacing = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_IndentSpacing: ImGui::GetStyle().IndentSpacing = value; break;
            case ImGuiStyleVar_CellPadding: ImGui::GetStyle().CellPadding = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_ScrollbarSize: ImGui::GetStyle().ScrollbarSize = value; break;
//            case ImGuiStyleVar_ScrollbarRounding: ImGui::GetStyle().ScrollbarRounding = value; break;
//            case ImGuiStyleVar_GrabMinSize: ImGui::GetStyle().GrabMinSize = value; break;
//            case ImGuiStyleVar_GrabRounding: ImGui::GetStyle().GrabRounding = value; break;
//            case ImGuiStyleVar_TabRounding: ImGui::GetStyle().TabRounding = value; break;
            case ImGuiStyleVar_ButtonTextAlign: ImGui::GetStyle().ButtonTextAlign = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_SelectableTextAlign: ImGui::GetStyle().SelectableTextAlign = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_SeparatorTextBorderSize: ImGui::GetStyle().SeparatorTextBorderSize = value; break;
            case ImGuiStyleVar_SeparatorTextAlign: ImGui::GetStyle().SeparatorTextAlign = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_SeparatorTextPadding: ImGui::GetStyle().SeparatorTextPadding = ImVec2(value[0], value[1]); break;
            default: throw std::invalid_argument(
                    b::format("ImGuiLua::ApplyStyleVec2Permanent: ImGuiStyleVar '{}' needs a float, not vec2",
                              enumValue));
        }
    }

    void ImGuiLua::ApplyStyleFloatPermanent(const std::string& enumValue, float value) {
        auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
        if (!stylevarEnum) {
            throw std::invalid_argument(
                    b::format("ImGuiLua::ApplyStyleFloatPermanent: Unknown ImGuiStyleVar '{}'", enumValue));
        }

        switch(stylevarEnum.value()) {
            case ImGuiStyleVar_Alpha: ImGui::GetStyle().Alpha = value; break;
            case ImGuiStyleVar_DisabledAlpha: ImGui::GetStyle().DisabledAlpha = value; break;
//            case ImGuiStyleVar_WindowPadding: ImGui::GetStyle().WindowPadding = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_WindowRounding: ImGui::GetStyle().WindowRounding = value; break;
            case ImGuiStyleVar_WindowBorderSize: ImGui::GetStyle().WindowBorderSize = value; break;
//            case ImGuiStyleVar_WindowMinSize: ImGui::GetStyle().WindowMinSize = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_WindowTitleAlign: ImGui::GetStyle().WindowTitleAlign = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_ChildRounding: ImGui::GetStyle().ChildRounding = value; break;
            case ImGuiStyleVar_ChildBorderSize: ImGui::GetStyle().ChildBorderSize = value; break;
            case ImGuiStyleVar_PopupRounding: ImGui::GetStyle().PopupRounding = value; break;
            case ImGuiStyleVar_PopupBorderSize: ImGui::GetStyle().PopupBorderSize = value; break;
//            case ImGuiStyleVar_FramePadding: ImGui::GetStyle().FramePadding = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_FrameRounding: ImGui::GetStyle().FrameRounding = value; break;
            case ImGuiStyleVar_FrameBorderSize: ImGui::GetStyle().FrameBorderSize = value; break;
//            case ImGuiStyleVar_ItemSpacing: ImGui::GetStyle().ItemSpacing = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_ItemInnerSpacing: ImGui::GetStyle().ItemInnerSpacing = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_IndentSpacing: ImGui::GetStyle().IndentSpacing = value; break;
//            case ImGuiStyleVar_CellPadding: ImGui::GetStyle().CellPadding = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_ScrollbarSize: ImGui::GetStyle().ScrollbarSize = value; break;
            case ImGuiStyleVar_ScrollbarRounding: ImGui::GetStyle().ScrollbarRounding = value; break;
            case ImGuiStyleVar_GrabMinSize: ImGui::GetStyle().GrabMinSize = value; break;
            case ImGuiStyleVar_GrabRounding: ImGui::GetStyle().GrabRounding = value; break;
            case ImGuiStyleVar_TabRounding: ImGui::GetStyle().TabRounding = value; break;
//            case ImGuiStyleVar_ButtonTextAlign: ImGui::GetStyle().ButtonTextAlign = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_SelectableTextAlign: ImGui::GetStyle().SelectableTextAlign = ImVec2(value[0], value[1]); break;
            case ImGuiStyleVar_SeparatorTextBorderSize: ImGui::GetStyle().SeparatorTextBorderSize = value; break;
//            case ImGuiStyleVar_SeparatorTextAlign: ImGui::GetStyle().SeparatorTextAlign = ImVec2(value[0], value[1]); break;
//            case ImGuiStyleVar_SeparatorTextPadding: ImGui::GetStyle().SeparatorTextPadding = ImVec2(value[0], value[1]); break;
            default: throw std::invalid_argument(
                        b::format("ImGuiLua::ApplyStyleVec2Permanent: ImGuiStyleVar '{}' needs a vec2, not float",
                                  enumValue));
        }
    }



    float ImGuiLua::GetFontSize() {
        return ImGui::GetFontSize();
    }



    void ImGuiLua::SetCursorPos(const ImVec2& pos) {
        ImGui::SetCursorPos(pos);
    }

    float ImGuiLua::GetCursorPosX() {
        return ImGui::GetCursorPosX();
    }

    float ImGuiLua::GetCursorPosY() {
        return ImGui::GetCursorPosY();
    }



    void ImGuiLua::SetNextItemWidth(float width) {
        ImGui::SetNextItemWidth(width);
    }

    ImVec2 ImGuiLua::GetWindowContentRegionMin() {
        return ImGui::GetWindowContentRegionMin();
    }

    ImVec2 ImGuiLua::GetWindowContentRegionMax() {
        return ImGui::GetWindowContentRegionMax();
    }

    void ImGuiLua::DeclareLuaBridge(lua_State* L) {
        luabridge::getGlobalNamespace(L)
                .beginClass<ImVec2>("ImVec2")
                .addConstructor<void(*)(float, float)>()
                .addProperty("x", &ImVec2::x)
                .addProperty("y", &ImVec2::y)
                .endClass();

        luabridge::getGlobalNamespace(L)
                .beginClass<ImVec4>("ImVec4")
                .addConstructor<void(*)(float, float, float, float)>()
                .addProperty("x", &ImVec4::x)
                .addProperty("y", &ImVec4::y)
                .addProperty("z", &ImVec4::z)
                .addProperty("w", &ImVec4::w)
                .endClass();

        luabridge::getGlobalNamespace(L)
                .beginClass<ImColor>("ImColor")
                .addConstructor<void(*)(int, int, int, int)>()
                .addProperty("Value", &ImColor::Value)
                .endClass();

        luabridge::getGlobalNamespace(L)
                .beginClass<ImGuiLua>("ImGui")

                .ADD_STATIC_FUNCTION(Begin)
                .ADD_STATIC_FUNCTION(End)

                .ADD_STATIC_FUNCTION(ShowDemoWindow)

                .ADD_STATIC_FUNCTION(Text)
                .ADD_STATIC_FUNCTION(Button)

                .ADD_STATIC_FUNCTION(ApplyStyleColorPermanent)
                .ADD_STATIC_FUNCTION(ApplyStyleVec2Permanent)
                .ADD_STATIC_FUNCTION(ApplyStyleFloatPermanent)

                .ADD_STATIC_FUNCTION(GetFontSize)
                .ADD_STATIC_FUNCTION(SetCursorPos)
                .ADD_STATIC_FUNCTION(GetCursorPosX)
                .ADD_STATIC_FUNCTION(GetCursorPosY)

                .ADD_STATIC_FUNCTION(SetNextItemWidth)
                .ADD_STATIC_FUNCTION(GetWindowContentRegionMin)
                .ADD_STATIC_FUNCTION(GetWindowContentRegionMax)
                .endClass();
    }

    lua_State* ImGuiLua::CreateLuaState() {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        DeclareLuaBridge(L);
        return L;
    }

    void ImGuiLua::RunLuaString(lua_State* L, const std::string& lua) {
        int scriptLoadStatus = luaL_dostring(L, lua.c_str());
        if (scriptLoadStatus != 0) {
            b::log::error("ImGuiLua::RunLuaString: {}", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    bool ImGuiLua::CallLuaFunction(lua_State* L, const std::string& function_name) {
        luabridge::LuaRef func = luabridge::getGlobal(L, function_name.c_str());
        if (func.isFunction()) {
            luabridge::LuaResult res = func();
            if (!res) {
                b::log::error("ImGuiLua::CallLuaFunction: {}() returned an error: {}",
                              function_name, res.errorMessage());
                return false;
            }
        }
        else {
            b::log::error("ImGuiLua::CallLuaFunction: function {}() does not exist", function_name);
            return false;
        }
        return true;
    }

}   // namespace b