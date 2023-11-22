#pragma once

#include <imgui.h>

struct SDL_Window;
struct SDL_Renderer;
typedef void *SDL_GLContext;

namespace b {

    class Renderer {
    public:
        explicit Renderer(SDL_Window* sdlWindow);
        ~Renderer();

        void clear(const ImColor& color);

        void render();

        // Prevent all move and assignment operations because of the raw pointers
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

    private:
        SDL_Window* m_sdlWindowReference {};  // This pointer is only a reference and DOES NOT OWN the window!!!
        SDL_Renderer* m_sdlRenderer {};
    };

}  // namespace b