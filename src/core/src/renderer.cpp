
#include "battery/renderer.hpp"
#include "SDL2/SDL.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "battery/constants.hpp"
#include "battery/embed.hpp"
#include "battery/log.hpp"
#include <optional>
#include <stdexcept>

#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"

namespace b {

RenderWindow::RenderWindow(const std::string& title,
                           const b::Vec2& size,
                           const WindowOptions& options)
    : Window(title, size, options)
{

    // Setup SDL renderer
    m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_sdlRenderer == nullptr) {
        throw std::runtime_error(
            b::format("b::Window: Failed to create SDL renderer: {}", SDL_GetError()));
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Setup ImGui
    if (!ImGui_ImplSDL2_InitForSDLRenderer(m_sdlWindow, m_sdlRenderer)) {
        throw std::runtime_error(b::format(
            "b::Window: Failed to init ImGui SDL2 backend: {}", SDL_GetError()));
    }
    if (!ImGui_ImplSDLRenderer2_Init(m_sdlRenderer)) {
        throw std::runtime_error(b::format(
            "b::Window: Failed to init ImGui SDLRenderer2 backend: {}", SDL_GetError()));
    }

    ImFontConfig fontCfg;
    fontCfg.FontDataOwnedByAtlas = false;
    auto ttf = b::embed<"resources/roboto-medium.ttf">();
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
        (void*)ttf.data(), ttf.size(), Constants::DefaultFontSize(), &fontCfg);
    ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts.back();

    m_luaState = b::ImGuiLua::CreateLuaState();
    bindEmbeddedLuaScript<"ui/default.lua">();
    bindEmbeddedLuaScript<"ui/widgets.lua">();
    m_numInternalLuaLoaders = m_luaLoaders.size();
}

RenderWindow::~RenderWindow()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (m_sdlRenderer != nullptr) {
        SDL_DestroyRenderer(m_sdlRenderer);
    }
}

void RenderWindow::clear(const ImColor& color)
{
    SDL_SetRenderDrawColor(m_sdlRenderer,
                           static_cast<uint8_t>(color.Value.x * 255.f),
                           static_cast<uint8_t>(color.Value.y * 255.f),
                           static_cast<uint8_t>(color.Value.z * 255.f),
                           static_cast<uint8_t>(color.Value.w * 255.f));
    SDL_RenderClear(m_sdlRenderer);
}

void RenderWindow::prepareFrame()
{
    // SDL_GL_SetSwapInterval(1);

    clear(b::Constants::DefaultWindowBackgroundColor());

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    luaErrorMessage = {};
    for (auto& loader : m_luaLoaders) {
        auto result = loader->updateScript(m_luaState);
        if (!result) {
            luaErrorMessage = result.error();
            break;
        }
    }
}

void RenderWindow::renderFrame()
{
    if (m_luaLoaders.size() > m_numInternalLuaLoaders && !luaErrorMessage) {
        auto result = callLuaFunctions();
        if (!result) {
            luaErrorMessage = result.error();
        }
    }

    ImGui::ErrorCheckEndFrameRecover(nullptr);

    if (luaErrorMessage) {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y), ImGuiCond_Always);
        ImGui::Begin("Error Output",
                     nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
                         | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", luaErrorMessage.value().c_str());
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_sdlRenderer);
}

std::expected<std::nullopt_t, std::string> RenderWindow::callLuaFunctions()
{
    auto result = ImGuiLua::CallLuaFunction(m_luaState, "preRender");
    if (!result) {
        return result;
    }
    result = ImGuiLua::CallLuaFunction(m_luaState, "render");
    if (!result) {
        return result;
    }
    result = ImGuiLua::CallLuaFunction(m_luaState, "postRender");
    if (!result) {
        return result;
    }
    return std::nullopt;
}

} // namespace b
