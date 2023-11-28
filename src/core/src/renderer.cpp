
#include "battery/embed.hpp"
#include "battery/renderer.hpp"
#include "battery/constants.hpp"
#include "battery/log.hpp"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "SDL2/SDL.h"
#include <stdexcept>

#include "imgui.h"
#include "imgui_internal.h"

namespace b {

    RenderWindow::RenderWindow(const std::string& title, const b::Vec2& size, const WindowOptions& options)
            : Window(title, size, options) {

        // Setup SDL renderer
        m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        if (m_sdlRenderer == nullptr) {
            throw std::runtime_error(b::format("b::Window: Failed to create SDL renderer: {}",
                                               SDL_GetError()));
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup ImGui
        if (!ImGui_ImplSDL2_InitForSDLRenderer(m_sdlWindow, m_sdlRenderer)) {
            throw std::runtime_error(b::format("b::Window: Failed to init ImGui SDL2 backend: {}",
                                               SDL_GetError()));
        }
        if (!ImGui_ImplSDLRenderer2_Init(m_sdlRenderer)) {
            throw std::runtime_error(b::format("b::Window: Failed to init ImGui SDLRenderer2 backend: {}",
                                               SDL_GetError()));
        }

        ImFontConfig fontCfg;
        fontCfg.FontDataOwnedByAtlas = false;
        auto ttf = b::embed<"resources/roboto-medium.ttf">();
        ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)ttf.data(), ttf.size(),
                                                   Constants::DefaultFontSize(), &fontCfg);
        ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts.back();

        m_luaState = b::ImGuiLua::CreateLuaState();
        bindEmbeddedLuaScript<"ui/default.lua">();
        bindEmbeddedLuaScript<"ui/widgets.lua">();
        m_numInternalLuaLoaders = m_luaLoaders.size();
    }

    RenderWindow::~RenderWindow() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        if (m_sdlRenderer != nullptr) {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
    }

    void RenderWindow::clear(const ImColor& color) {
        SDL_SetRenderDrawColor(m_sdlRenderer,
                               static_cast<uint8_t>(color.Value.x * 255.f),
                               static_cast<uint8_t>(color.Value.y * 255.f),
                               static_cast<uint8_t>(color.Value.z * 255.f),
                               static_cast<uint8_t>(color.Value.w * 255.f));
        SDL_RenderClear(m_sdlRenderer);
    }

    void RenderWindow::prepareFrame() {
        SDL_GL_SetSwapInterval(1);

        clear(b::Constants::DefaultWindowBackgroundColor());

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        for (auto& loader : m_luaLoaders) {
            loader->updateScript(m_luaState);
        }
    }

    void RenderWindow::renderFrame() {
        if (m_luaLoaders.size() > m_numInternalLuaLoaders) {
            LuaLoader::invokeRender(m_luaState);
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(m_sdlRenderer);
    }

}   // namespace b