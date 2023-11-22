
#include "battery/renderer.hpp"
#include "battery/constants.hpp"
#include "battery/log.hpp"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "SDL2/SDL.h"
#include <stdexcept>

namespace b {

    Renderer::Renderer(SDL_Window* sdlWindow) : m_sdlWindowReference(sdlWindow) {

        // Setup SDL renderer
        m_sdlRenderer = SDL_CreateRenderer(m_sdlWindowReference, -1, SDL_RENDERER_ACCELERATED);
        if (m_sdlRenderer == nullptr) {
            throw std::runtime_error(b::format("b::Window: Failed to create SDL renderer: {}",
                                               SDL_GetError()));
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup ImGui
        if (!ImGui_ImplSDL2_InitForSDLRenderer(m_sdlWindowReference, m_sdlRenderer)) {
            throw std::runtime_error(b::format("b::Window: Failed to init ImGui SDL2 backend: {}",
                                               SDL_GetError()));
        }
        if (!ImGui_ImplSDLRenderer2_Init(m_sdlRenderer)) {
            throw std::runtime_error(b::format("b::Window: Failed to init ImGui SDLRenderer2 backend: {}",
                                               SDL_GetError()));
        }
    }

    Renderer::~Renderer() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        if (m_sdlRenderer != nullptr) {
            SDL_DestroyRenderer(m_sdlRenderer);
        }
    }

    void Renderer::clear(const ImColor& color) {
        SDL_SetRenderDrawColor(m_sdlRenderer,
                               static_cast<uint8_t>(color.Value.x * 255.f),
                               static_cast<uint8_t>(color.Value.y * 255.f),
                               static_cast<uint8_t>(color.Value.z * 255.f),
                               static_cast<uint8_t>(color.Value.w * 255.f));
        SDL_RenderClear(m_sdlRenderer);
    }

    void Renderer::render() {
        SDL_GL_SetSwapInterval(1);

        clear(b::Constants::DefaultWindowBackgroundColor());

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Here do something
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(m_sdlRenderer);

//        SDL_GL_SwapWindow(m_sdlWindowReference);
    }

}   // namespace b