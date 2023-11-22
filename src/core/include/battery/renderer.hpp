#pragma once

#include <imgui.h>
#include "window.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void *SDL_GLContext;

namespace b {

    class RenderWindow : public Window {
    public:
        RenderWindow(const std::string& title, const b::Vec2& size, std::optional<WindowOptions> options = {});
        ~RenderWindow();

        void clear(const ImColor& color);

        void prepareFrame();
        void renderFrame();

        // Prevent all move and assignment operations because of the raw pointers
        RenderWindow(const RenderWindow&) = delete;
        RenderWindow& operator=(const RenderWindow&) = delete;
        RenderWindow(RenderWindow&&) = delete;
        RenderWindow& operator=(RenderWindow&&) = delete;

    private:
        SDL_Renderer* m_sdlRenderer {};
    };

}  // namespace b