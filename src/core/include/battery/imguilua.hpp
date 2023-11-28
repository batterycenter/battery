#pragma once

#include "constexpr.hpp"
#include "battery/embed.hpp"
#include "imgui.h"
#include <string>
#include <mutex>

struct lua_State;

namespace b {

    class LuaLoader {
    public:
        LuaLoader() = default;

        template<embed_string_literal identifier>
        void bindEmbeddedLuaScript() {
            if (m_haveLuaResource) {
                throw std::logic_error("b::LuaLoader::loadEmbeddedLuaScript(): "
                                       "Lua script already loaded, cannot bind another resource to the same loader");
            }
            m_luaResource = b::embed<identifier>();
            m_debugFilename = identifier.value;
            m_luaResource.get([this](const auto& file) {
                std::lock_guard const lock(m_luaResourceMutex);
                m_luaResourceString = file.str();
                m_luaResourceChanged = true;
            });
            m_haveLuaResource = true;
        }

        void updateScript(lua_State* luaState);
        static void invokeRender(lua_State* luaState);

        // Prevent all move and assignment operations to avoid issues with the lambda callback
        LuaLoader(const LuaLoader&) = delete;
        LuaLoader& operator=(const LuaLoader&) = delete;
        LuaLoader(LuaLoader&&) = delete;
        LuaLoader& operator=(LuaLoader&&) = delete;

    private:
        bool m_haveLuaResource = false;
        bool m_luaResourceChanged = false;
        std::string m_debugFilename {};
        std::mutex m_luaResourceMutex {};
        std::string m_luaResourceString {};
        b::EmbedInternal::EmbeddedFile m_luaResource {};
    };

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

        static void RunLuaString(lua_State* L, const std::string& lua,
                                 const std::string& luaDebugFilename = {});

        static bool CallLuaFunction(lua_State* L, const std::string& function_name,
                                    const std::string& luaDebugFilename = {});

        ImGuiLua() = delete;
    };

} // namespace b