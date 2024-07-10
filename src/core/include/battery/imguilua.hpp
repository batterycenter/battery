#pragma once

#include "battery/embed.hpp"
#include "constexpr.hpp"
#include "imgui.h"
#include <expected>
#include <mutex>
#include <optional>
#include <string>

struct lua_State;

namespace b {

class LuaLoader {
public:
    LuaLoader() = default;

    template <embed_string_literal identifier> void bindEmbeddedLuaScript()
    {
        if (m_haveLuaResource) {
            throw std::logic_error("b::LuaLoader::loadEmbeddedLuaScript(): "
                                   "Lua script already loaded, cannot bind another "
                                   "resource to the same loader");
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

    std::expected<std::nullopt_t, std::string> updateScript(lua_State* luaState);

    // Prevent all move and assignment operations to avoid issues with the lambda
    // callback
    LuaLoader(const LuaLoader&) = delete;
    LuaLoader& operator=(const LuaLoader&) = delete;
    LuaLoader(LuaLoader&&) = delete;
    LuaLoader& operator=(LuaLoader&&) = delete;

private:
    bool m_haveLuaResource = false;
    bool m_luaResourceChanged = false;
    std::string m_debugFilename;
    std::mutex m_luaResourceMutex;
    std::string m_luaResourceString;
    b::EmbedInternal::EmbeddedFile m_luaResource {};
};

class ImGuiLua {
public:
    static void DeclareLuaBridge(lua_State* L);

    static lua_State* CreateLuaState();

    static std::expected<std::nullopt_t, std::string>
    RunLuaString(lua_State* L,
                 const std::string& lua,
                 const std::string& luaDebugFilename = {});

    static std::expected<std::nullopt_t, std::string>
    CallLuaFunction(lua_State* L,
                    const std::string& function_name,
                    const std::string& luaDebugFilename = {});

    ImGuiLua() = delete;
};

} // namespace b
