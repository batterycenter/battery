#pragma once

#include <imgui.h>
#include "window.hpp"
#include "battery/embed.hpp"
#include "battery/imguilua.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void *SDL_GLContext;

namespace b {

    class RenderWindow : public Window {
    public:
        RenderWindow(const std::string& title, const b::Vec2& size, const WindowOptions& options = WindowOptions());
        virtual ~RenderWindow();

        void clear(const ImColor& color);

        void prepareFrame();
        void renderFrame();

        template<embed_string_literal identifier>
        void bindEmbeddedLuaScript() {
            m_luaLoaders.emplace_back(std::make_unique<LuaLoader>());
            m_luaLoaders.back()->bindEmbeddedLuaScript<identifier>();
        }

        // Prevent all move and assignment operations because of the raw pointers
        RenderWindow(const RenderWindow&) = delete;
        RenderWindow& operator=(const RenderWindow&) = delete;
        RenderWindow(RenderWindow&&) = delete;
        RenderWindow& operator=(RenderWindow&&) = delete;

    private:
        SDL_Renderer* m_sdlRenderer {};

        lua_State* m_luaState;
        std::vector<std::unique_ptr<LuaLoader>> m_luaLoaders;
        size_t m_numInternalLuaLoaders = 0;
    };

}  // namespace b
