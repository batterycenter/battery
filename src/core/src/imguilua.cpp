
#include "battery/imguilua.hpp"
#include "battery/log.hpp"
#include "battery/application.hpp"
#include "battery/extern/magic_enum.hpp"
#include "imgui_internal.h"

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#define ADD_STATIC_IMGUI_FUNCTION(name) \
        addStaticFunction(#name, &ImGui::name)

#define ADD_STATIC_IMGUILUA_FUNCTION(name) \
        addStaticFunction(#name, &ImGuiLua::name)

namespace b {

    void LuaLoader::updateScript(lua_State* luaState) {
        if (m_haveLuaResource) {
            std::lock_guard const lock(m_luaResourceMutex);
            if (m_luaResourceChanged) {
                m_luaResourceChanged = false;
                b::log::info("Lua Script reloaded");
                b::ImGuiLua::RunLuaString(luaState, m_luaResourceString, m_debugFilename);
            }
        }
    }

    void LuaLoader::invokeRender(lua_State* luaState) {
        b::ImGuiLua::CallLuaFunction(luaState, "render");
        ImGui::ErrorCheckEndFrameRecover(nullptr);
    }

    bool ImGuiLua::Begin(const std::string &name) {
        return ImGui::Begin(name.c_str());
    }

    void ImGuiLua::End() {
        ImGui::End();
    }

    void ImGuiLua::ShowDemoWindow() {
        ImGui::ShowDemoWindow();
    }

    void ImGuiLua::Text(const std::string& text) {
        ImGui::Text("%s", text.c_str());
    }

    bool ImGuiLua::Button(const std::string& text, const ImVec2& size) {
        return ImGui::Button(text.c_str(), size);
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

    void ImGuiLua::RequestAnimationFrameIn(double seconds) {
        b::Application::get().requestAnimationFrameIn(seconds);
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
                .beginNamespace("b")
                .addFunction("requestAnimationFrameIn", &ImGuiLua::RequestAnimationFrameIn)
                .endNamespace();

        luabridge::getGlobalNamespace(L)
                .beginClass<ImGuiLua>("ImGui")

                .ADD_STATIC_IMGUILUA_FUNCTION(Begin)
                .ADD_STATIC_IMGUILUA_FUNCTION(End)

                .ADD_STATIC_IMGUILUA_FUNCTION(ShowDemoWindow)

                .ADD_STATIC_IMGUILUA_FUNCTION(Text)
                .ADD_STATIC_IMGUILUA_FUNCTION(Button)

                .ADD_STATIC_IMGUILUA_FUNCTION(ApplyStyleColorPermanent)
                .ADD_STATIC_IMGUILUA_FUNCTION(ApplyStyleVec2Permanent)
                .ADD_STATIC_IMGUILUA_FUNCTION(ApplyStyleFloatPermanent)

                .ADD_STATIC_IMGUI_FUNCTION(GetFontSize)
                .ADD_STATIC_IMGUI_FUNCTION(SetCursorPos)
                .ADD_STATIC_IMGUI_FUNCTION(GetCursorPosX)
                .ADD_STATIC_IMGUI_FUNCTION(GetCursorPosY)

                .ADD_STATIC_IMGUI_FUNCTION(SetNextItemWidth)
                .ADD_STATIC_IMGUI_FUNCTION(GetWindowContentRegionMin)
                .ADD_STATIC_IMGUI_FUNCTION(GetWindowContentRegionMax)

                .endClass();
    }

    lua_State* ImGuiLua::CreateLuaState() {
        lua_State* state = luaL_newstate();
        luaL_openlibs(state);
        DeclareLuaBridge(state);
        return state;
    }

    void ImGuiLua::RunLuaString(lua_State* L, const std::string& lua, const std::string& luaDebugFilename) {
        int scriptLoadStatus = luaL_dostring(L, lua.c_str());
        if (scriptLoadStatus != 0) {
            if (!luaDebugFilename.empty()) {
                b::log::error("File {}: {}", luaDebugFilename, lua_tostring(L, -1));
            }
            else {
                b::log::error("b::ImGuiLua::RunLuaString(): {}", lua_tostring(L, -1));
            }
            lua_pop(L, 1);
        }
    }

    bool ImGuiLua::CallLuaFunction(lua_State* L, const std::string& functionName, const std::string& luaDebugFilename) {
        luabridge::LuaRef const func = luabridge::getGlobal(L, functionName.c_str());
        if (func.isFunction()) {
            luabridge::LuaResult const res = func();
            if (!res) {
                if (!luaDebugFilename.empty()) {
                    b::log::error("File {}: {}() returned an error: {}", luaDebugFilename, functionName, res.errorMessage());
                }
                else {
                    b::log::error("b::ImGuiLua::CallLuaFunction(): {}() returned an error: {}",
                                  functionName, res.errorMessage());
                }
                return false;
            }
        }
        else {
            b::log::error("ImGuiLua::CallLuaFunction: function {}() does not exist", functionName);
            return false;
        }
        return true;
    }

}   // namespace b