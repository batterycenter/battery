
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

#define ADD_IMGUI_FUNCTION(name) \
        addFunction(#name, &ImGui::name)

#define ADD_FUNCTION(name) \
        addFunction(#name, &name)

#define NS_ADD_ENUM(enum) \
        for (auto [enumValue, enumKey] : magic_enum::enum_entries<enum>()) { \
            ns.addVariable(enumKey.data(), enumValue); \
        }

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

    void Text(const std::string& text) {
        ImGui::Text("%s", text.c_str());
    }

    bool Button(const std::string& text, const ImVec2& size) {
        return ImGui::Button(text.c_str(), size);
    }


    void ApplyStyleColorPermanent(const std::string& enumValue, const ImColor& color) {
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

    void ApplyStyleVec2Permanent(const std::string& enumValue, const ImVec2& value) {
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

    void ApplyStyleFloatPermanent(const std::string& enumValue, float value) {
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

    void PushStyleColor(const std::string& enumValue, const ImColor& color) {
        auto stylevarEnum = magic_enum::enum_cast<ImGuiCol_>(enumValue);
        if (!stylevarEnum) {
            throw std::invalid_argument(b::format("ImGuiLua::PushStyleColor: Unknown ImGuiStyleCol '{}'", enumValue));
        }
        ImGui::PushStyleColor(*stylevarEnum, color.Value);
    }

    void PushStyleVarVec2(const std::string& enumValue, const ImVec2& value) {
        auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
        if (!stylevarEnum) {
            throw std::invalid_argument(b::format("ImGuiLua::PushStyleVarVec2: Unknown ImGuiStyleVar '{}'", enumValue));
        }

        // Check if the type is really vec2
        auto* const varInfo = ImGui::GetStyleVarInfo(*stylevarEnum);
        if (varInfo->Type == ImGuiDataType_Float && varInfo->Count == 2) {
            ImGui::PushStyleVar(*stylevarEnum, value);
        }
        else {
            b::log::error("ImGuiLua::PushStyleVarVec2: ImGuiStyleVar '{}' "
                          "needs a vec2, not float", enumValue);
            // We push a value regardless that has no effect, to not break PopStyleVar()
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding);
        }
    }

    void PushStyleVarFloat(const std::string& enumValue, float value) {
        auto stylevarEnum = magic_enum::enum_cast<ImGuiStyleVar_>(enumValue);
        if (!stylevarEnum) {
            throw std::invalid_argument(b::format("ImGuiLua::PushStyleVarFloat: Unknown ImGuiStyleVar '{}'", enumValue));
        }

        // Check if the type is really float
        auto* const varInfo = ImGui::GetStyleVarInfo(*stylevarEnum);
        if (varInfo->Type == ImGuiDataType_Float && varInfo->Count == 1) {
            ImGui::PushStyleVar(*stylevarEnum, value);
        }
        else {
            b::log::error("ImGuiLua::PushStyleVarFloat: ImGuiStyleVar '{}' "
                          "needs a float, not vec2", enumValue);
            // We push a value regardless that has no effect, to not break PopStyleVar()
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha);
        }
    }

    void PopStyleColor() {
        ImGui::PopStyleColor();
    }

    void PopStyleVarVec2() {
        ImGui::PopStyleVar();
    }

    void PopStyleVarFloat() {
        ImGui::PopStyleVar();
    }

    void RequestAnimationFrameIn(double seconds) {
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
                .ADD_FUNCTION(RequestAnimationFrameIn)
                .endNamespace();

        auto ns = luabridge::getGlobalNamespace(L).beginNamespace("ImGui");
        NS_ADD_ENUM(ImGuiWindowFlags_);
        NS_ADD_ENUM(ImGuiButtonFlags_);
        NS_ADD_ENUM(ImGuiColorEditFlags_);
        NS_ADD_ENUM(ImGuiInputFlags_);
        NS_ADD_ENUM(ImGuiPopupFlags_);
        NS_ADD_ENUM(ImGuiScrollFlags_);
        NS_ADD_ENUM(ImGuiSelectableFlags_);
        NS_ADD_ENUM(ImGuiSeparatorFlags_);
        NS_ADD_ENUM(ImGuiSliderFlags_);
        NS_ADD_ENUM(ImGuiTabBarFlags_);
        NS_ADD_ENUM(ImGuiTabItemFlags_);
        NS_ADD_ENUM(ImGuiTextFlags_);

        ns.addFunction("Begin", [](const std::string& name, int flags) {
            return ImGui::Begin(name.c_str(), nullptr, flags);
        });
        ns.addFunction("End", &ImGui::End);
        ns.addFunction("BeginChild", [](const std::string &name, const ImVec2& size, bool border, int flags) -> bool {
            return ImGui::BeginChild(name.c_str(), size, border, flags);
        });
        ns.addFunction("EndChild", &ImGui::EndChild);
        ns.addFunction("PushIDInt", [](int id) {
            ImGui::PushID(id);
        });
        ns.addFunction("PushIDStr", [](const std::string& id) {
            ImGui::PushID(id.c_str());
        });
        ns.addFunction("PopID", &ImGui::PopID);
        ns.addFunction("ShowDemoWindow", []() { ImGui::ShowDemoWindow(); });

        ns.ADD_FUNCTION(Text);
        ns.ADD_FUNCTION(Button);

        ns.ADD_FUNCTION(ApplyStyleColorPermanent);
        ns.ADD_FUNCTION(ApplyStyleVec2Permanent);
        ns.ADD_FUNCTION(ApplyStyleFloatPermanent);

        ns.ADD_FUNCTION(PushStyleColor);
        ns.ADD_FUNCTION(PushStyleVarVec2);
        ns.ADD_FUNCTION(PushStyleVarFloat);

        ns.ADD_FUNCTION(PopStyleColor);
        ns.ADD_FUNCTION(PopStyleVarVec2);
        ns.ADD_FUNCTION(PopStyleVarFloat);

        ns.ADD_IMGUI_FUNCTION(GetFontSize);
        ns.ADD_IMGUI_FUNCTION(SetCursorPos);
        ns.ADD_IMGUI_FUNCTION(GetCursorPosX);
        ns.ADD_IMGUI_FUNCTION(GetCursorPosY);

        ns.ADD_IMGUI_FUNCTION(SetNextItemWidth);
        ns.ADD_IMGUI_FUNCTION(GetWindowContentRegionMin);
        ns.ADD_IMGUI_FUNCTION(GetWindowContentRegionMax);

        ns.endNamespace();
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