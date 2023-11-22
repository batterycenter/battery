
#include "battery/renderwindow.hpp"
#include "SDL2/SDL.h"

//#include "imgui_internal.h"

namespace b {

    RenderWindow::RenderWindow() {

    }

    RenderWindow::~RenderWindow() noexcept {

    }

    void RenderWindow::clear(const ImColor& color) {
        SDL_SetRenderDrawColor(m_sdlRenderer,
                               static_cast<uint8_t>(color.Value.x * 255.f),
                               static_cast<uint8_t>(color.Value.y * 255.f),
                               static_cast<uint8_t>(color.Value.z * 255.f),
                               static_cast<uint8_t>(color.Value.w * 255.f));
        SDL_RenderClear(m_sdlRenderer);
    }

//    void RenderWindow::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
//        getRenderWindow().draw(drawable, states);
//    }
//
//    void RenderWindow::draw(const sf::Vertex* vertices,
//                      std::size_t vertexCount,
//                      sf::PrimitiveType type,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertices, vertexCount, type, states);
//    }
//
//    void RenderWindow::draw(const sf::VertexBuffer& vertexBuffer,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertexBuffer, states);
//    }
//
//    void RenderWindow::draw(const sf::VertexBuffer& vertexBuffer,
//                      std::size_t         firstVertex,
//                      std::size_t         vertexCount,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertexBuffer, firstVertex, vertexCount, states);
//    }
//
//    const sf::View& RenderWindow::getDefaultView() const {
//        return getRenderWindow().getDefaultView();
//    }
//
//    void RenderWindow::setView(const sf::View& view) {
//        getRenderWindow().setView(view);
//    }
//
//    const sf::View& RenderWindow::getView() const {
//        return getRenderWindow().getView();
//    }
//
//    sf::IntRect RenderWindow::getViewport(const sf::View& view) const {
//        return getRenderWindow().getViewport(view);
//    }
//
//    b::Vec2 RenderWindow::mapCoordsToPixel(const b::Vec2& point) const {
//        return getRenderWindow().mapCoordsToPixel(point);
//    }
//
//    b::Vec2 RenderWindow::mapCoordsToPixel(const b::Vec2& point, const sf::View& view) const {
//        return getRenderWindow().mapCoordsToPixel(point, view);
//    }
//
//    b::Vec2 RenderWindow::mapPixelToCoords(const b::Vec2& pixel) const {
//        return getRenderWindow().mapPixelToCoords(pixel);
//    }
//
//    b::Vec2 RenderWindow::mapPixelToCoords(const b::Vec2& pixel, const sf::View& view) const {
//        return getRenderWindow().mapPixelToCoords(pixel, view);
//    }
//
//    void RenderWindow::setJoystickThreshold(float threshold) {
//        getRenderWindow().setJoystickThreshold(threshold);
//    }
//
//    void RenderWindow::setKeyRepeatEnabled(bool enabled) {
//        getRenderWindow().setKeyRepeatEnabled(enabled);
//    }
//
//    void RenderWindow::setMouseCursorGrabbed(bool grabbed) {
//        getRenderWindow().setMouseCursorGrabbed(grabbed);
//    }
//
//    void RenderWindow::setMouseCursorVisible(bool visible) {
//        getRenderWindow().setMouseCursorVisible(visible);
//    }
//
////    void RenderWindow::setMouseCursor(const sf::Cursor& cursor) {
////        getRenderWindow().setMouseCursor(cursor);
////    }
//
//    void RenderWindow::setVerticalSyncEnabled(bool enabled) {
//        getRenderWindow().setVerticalSyncEnabled(enabled);
//    }
//
//    void RenderWindow::setVisible(bool visible) {
//        getRenderWindow().setVisible(visible);
//    }

//    void RenderWindow::invokeUpdate() {
//
//        processWindowEvents();
//        b::update_themes();     // TODO: Move this to a better place
//
//        try {
//            onUpdate();
//            prepareFrontendScript();
//        }
//        catch (const std::exception &e) {
//            recoverImGuiStacks();
//            m_errorMessage = e.what();
//            b::log::core::error("Unhandled exception:\n{}", e.what());
//        }
//
//        if (isOpen()) {
//            m_lastWindowState.position = getPosition();
//            m_lastWindowState.size = getSize();
//            m_lastWindowState.maximized = isMaximized();
//        }
//    }
//
//    void RenderWindow::invokeRender() {
//        clear(b::Constants::DefaultWindowBackgroundColor());
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL2_NewFrame(window);
//        ImGui::NewFrame();
//
//        b::LockFontStack();
//        renderContent();
//        b::UnlockFontStack();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        getRenderWindow().display();
//    }
//
//    void RenderWindow::recoverImGuiStacks() {
//        ImGui::ErrorCheckEndFrameRecover(nullptr);
//        ImGuiContext& g = *GImGui;
//
//        if (g.CurrentWindowStack.empty()) {
//            return;
//        }
//
//        ImGuiStackSizes* stackSizes = &g.CurrentWindowStack.back().StackSizesOnBegin;
//        while (g.FontStack.Size > stackSizes->SizeOfFontStack) {
//            ImGui::PopFont();
//        }
//    }

//    void RenderWindow::renderContent() {
//        if (m_errorMessage.has_value()) {
//            renderErrorMessage(m_errorMessage.value());
//            return;
//        }
//
//        try {
//            style.push();
//            onRender();
//            invokeFrontendScript();
//            style.pop();
//        }
//        catch (const std::exception &e) {
//            recoverImGuiStacks();
//            renderErrorMessage(e.what());
//            b::log::core::error("Unhandled exception:\n{}", e.what());
//        }
//    }

//    void Window::renderErrorMessage(const b::string& error) {
//        m_errorPanelWidget.childrenStyle.font = "default";  // Ask it to explicitly push the default font, in case it wasn't cleaned up properly
//        m_errorPanelWidget.left = 0;
//        m_errorPanelWidget.top = 0;
//        m_errorPanelWidget.width = getSize().x;
//        m_errorPanelWidget.height = getSize().y;
//        m_errorPanelWidget.flags.noTitleBar = true;
//        m_errorPanelWidget.flags.noMove = true;
//        m_errorPanelWidget.flags.noResize = true;
//        m_errorPanelWidget.style["ImGuiCol_WindowBg"] = "#333333"_u;
//        m_errorPanelWidget.style["ImGuiStyleVar_WindowRounding"] = 0;
//        m_errorTextWidget.label = b::format("Unhandled exception:\n{}", error);
//        m_errorTextWidget.left = "5px"_u;
//        m_errorTextWidget.top = "5px"_u;
//        m_errorTextWidget.right = "5px"_u;
//        m_errorTextWidget.bottom = "5px"_u;
//        m_errorTextWidget.alignv = b::AlignV::Top;
//        m_errorTextWidget.style["ImGuiCol_Text"] = "#D34040"_u;
//
//        m_errorPanelWidget([this]() {
//            m_errorTextWidget();
//        });
//    }
//
//    void Window::attach() {
//        m_isAttached = true;
//        onAttach();
//    }
//
//    void Window::detach() {
//        onDetach();
//        close();
//        m_isAttached = false;
//    }

}