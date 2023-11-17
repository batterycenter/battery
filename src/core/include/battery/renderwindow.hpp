#pragma once

#include "battery/window.hpp"
#include "imgui.h"

namespace b {

    class RenderWindow : public b::Window {
    public:

        RenderWindow();
        virtual ~RenderWindow() noexcept;

        // Prevent all move and assignment operations due to the reference
        RenderWindow(const RenderWindow&) = delete;
        RenderWindow& operator=(const RenderWindow&) = delete;
        RenderWindow(RenderWindow&&) = delete;
        RenderWindow& operator=(RenderWindow&&) = delete;

        void clear(const ImColor& color = ImColor(0, 0, 0, 255));

//        void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
//
//        void draw(const sf::Vertex*       vertices,
//                  std::size_t         vertexCount,
//                  sf::PrimitiveType       type,
//                  const sf::RenderStates& states = sf::RenderStates::Default);
//
//        void draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
//
//        void draw(const sf::VertexBuffer& vertexBuffer,
//                  std::size_t         firstVertex,
//                  std::size_t         vertexCount,
//                  const sf::RenderStates& states = sf::RenderStates::Default);
//
//        const sf::View& getDefaultView() const;
//
//        void setView(const sf::View& view);
//        const sf::View& getView() const;
//
//        sf::IntRect getViewport(const sf::View& view) const;
//
//        b::Vec2 mapCoordsToPixel(const b::Vec2& point) const;
//        b::Vec2 mapCoordsToPixel(const b::Vec2& point, const sf::View& view) const;
//
//        b::Vec2 mapPixelToCoords(const b::Vec2& pixel) const;
//        b::Vec2 mapPixelToCoords(const b::Vec2& pixel, const sf::View& view) const;

    protected:
        virtual void prepareFrontendScript() {}
        virtual void invokeFrontendScript() {}
        void recoverImGuiStacks();

        std::optional<std::string> m_errorMessage;

    private:
        void processEvent(SDL_Event* event) override;

        void renderContent();
        void updateWin32DarkMode();
        void processWindowEvents();
        void renderErrorMessage(const std::string& error);
        void attach();
        void detach();

//        b::widgets::panel m_errorPanelWidget;
//        b::widgets::text m_errorTextWidget;

    };

//    template<b::derived_from<b::PyContext> T, b::template_string_literal ContextName>
//    class PyWindow : public Window {
//    public:
//        T context;
//
//        PyWindow() {
//            context.m_initPyWindowFunc = [this](const b::py::function& python_ui_loop) {
//                pyInit(python_ui_loop);
//            };
//        }
//
//        void pyInit(const py::function& frontendCallback) {
//            m_pythonUiLoopCallback = frontendCallback;
//        }
//
//        template<typename T>
//        void loadPythonFrontendScriptResource(const T& script) {
//            m_uiScriptResourceLoader = std::make_unique<b::ResourceLoader>(script, [this](const auto& resource) {
//                m_uiScriptResource = resource;
//                m_uiScriptLoaded = false;
//            });
//        }
//
//        void prepareFrontendScript() override {    // Load python if not loaded already
//            if (!m_uiScriptResource.string().empty() && !m_uiScriptLoaded) {
//                m_uiScriptLoaded = false;
//                b::py::exec(m_uiScriptResource.string());
//                m_uiScriptLoaded = true;
//                m_errorMessage = {};
//            }
//        }
//
//        void invokeFrontendScript() override {
//
//        }
//
//        void definePythonBindings(b::py::module& module) override {
//            T::defineParentPythonClass(module);
//            context.definePythonClass(module);
//            module.attr(ContextName.value) = &context;
//        }
//
//    private:
//        py::function m_pythonUiLoopCallback;
//        b::Resource m_uiScriptResource;
//        std::unique_ptr<b::ResourceLoader> m_uiScriptResourceLoader;
//        bool m_uiScriptLoaded = true;
//    };

}