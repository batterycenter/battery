#pragma once

#include "battery/embed.hpp"
#include "battery/imguilua.hpp"
#include "window.hpp"
#include <imgui.h>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <LuaBridge/Array.h>
#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/Vector.h>

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;

namespace b {

class RenderWindow : public Window {
public:
    RenderWindow(const std::string& title,
                 const b::Vec2& size,
                 const WindowOptions& options = WindowOptions());
    virtual ~RenderWindow();

    void clear(const ImColor& color);

    void prepareFrame();
    void renderFrame();

    template <typename TFunc>
    void makeFunctionAvailable(const std::string& name, TFunc&& func)
    {
        luabridge::getGlobalNamespace(m_luaState)
            .beginNamespace("App")
            .addFunction(name.c_str(), func)
            .endNamespace();
    }

    lua_State* getLuaState()
    {
        return m_luaState;
    }

    template <embed_string_literal identifier> void bindEmbeddedLuaScript()
    {
        m_luaLoaders.emplace_back(std::make_unique<LuaLoader>());
        m_luaLoaders.back()->bindEmbeddedLuaScript<identifier>();
    }

    // Prevent all move and assignment operations because of the raw pointers
    RenderWindow(const RenderWindow&) = delete;
    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

private:
    std::expected<std::nullopt_t, std::string> callLuaFunctions();

    SDL_Renderer* m_sdlRenderer {};

    lua_State* m_luaState;
    std::vector<std::unique_ptr<LuaLoader>> m_luaLoaders;
    std::optional<std::string> luaErrorMessage;
    size_t m_numInternalLuaLoaders = 0;
};

} // namespace b
