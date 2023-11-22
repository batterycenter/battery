#pragma once

#include "imgui.h"
#include <string>

struct lua_State;

namespace b {

    class ImGuiLua {
    public:
        static bool Begin(const std::string &name);
        static void End();

        static void ShowDemoWindow();

        static void Text(const std::string &text);
        static bool Button(const std::string &text, const ImVec2& size);

        static void ApplyStyleColorPermanent(const std::string& enumValue, const ImColor& color);
        static void ApplyStyleVec2Permanent(const std::string& enumValue, const ImVec2& value);
        static void ApplyStyleFloatPermanent(const std::string& enumValue, float value);

        static float GetFontSize();
        static void SetCursorPos(const ImVec2& pos);
        static float GetCursorPosX();
        static float GetCursorPosY();

        static void SetNextItemWidth(float width);
        static ImVec2 GetWindowContentRegionMin();
        static ImVec2 GetWindowContentRegionMax();

        static void DeclareLuaBridge(lua_State* L);
        static lua_State* CreateLuaState();
        static void RunLuaString(lua_State* L, const std::string& lua);
        static bool CallLuaFunction(lua_State* L, const std::string& function_name);

        ImGuiLua() = delete;
    };

} // namespace b