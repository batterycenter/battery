//
//#include "battery/renderwindow.hpp"
//#include "backends/imgui_impl_sdl2.h"
//#include "backends/imgui_impl_opengl3.h"
//
////#include "imgui_internal.h"
//
//#ifdef B_OS_WINDOWS
//#include <Windows.h>
//#include <dwmapi.h>
//#endif
//
//namespace b {
//
//    RenderWindow::RenderWindow() {
//        // setup Dear ImGui context
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO(); (void)io;
////        io.Fonts->AddFontFromFileTTF("verdana.ttf", 18.0f, NULL, NULL);
//
//        setImGuiStyle();
//
//        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
//        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
//    }
//
//    RenderWindow::~RenderWindow() noexcept {
//
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplSDL2_Shutdown();
//        ImGui::DestroyContext();
//    }
//
//    void Window::clear(const sf::Color& color) {
//        getRenderWindow().clear(color);
//    }
//
//    void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
//        getRenderWindow().draw(drawable, states);
//    }
//
//    void Window::draw(const sf::Vertex* vertices,
//                      std::size_t vertexCount,
//                      sf::PrimitiveType type,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertices, vertexCount, type, states);
//    }
//
//    void Window::draw(const sf::VertexBuffer& vertexBuffer,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertexBuffer, states);
//    }
//
//    void Window::draw(const sf::VertexBuffer& vertexBuffer,
//                      std::size_t         firstVertex,
//                      std::size_t         vertexCount,
//                      const sf::RenderStates& states) {
//        getRenderWindow().draw(vertexBuffer, firstVertex, vertexCount, states);
//    }
//
//    const sf::View& Window::getDefaultView() const {
//        return getRenderWindow().getDefaultView();
//    }
//
//    void Window::setView(const sf::View& view) {
//        getRenderWindow().setView(view);
//    }
//
//    const sf::View& Window::getView() const {
//        return getRenderWindow().getView();
//    }
//
//    sf::IntRect Window::getViewport(const sf::View& view) const {
//        return getRenderWindow().getViewport(view);
//    }
//
//    b::Vec2 Window::mapCoordsToPixel(const b::Vec2& point) const {
//        return getRenderWindow().mapCoordsToPixel(point);
//    }
//
//    b::Vec2 Window::mapCoordsToPixel(const b::Vec2& point, const sf::View& view) const {
//        return getRenderWindow().mapCoordsToPixel(point, view);
//    }
//
//    b::Vec2 Window::mapPixelToCoords(const b::Vec2& pixel) const {
//        return getRenderWindow().mapPixelToCoords(pixel);
//    }
//
//    b::Vec2 Window::mapPixelToCoords(const b::Vec2& pixel, const sf::View& view) const {
//        return getRenderWindow().mapPixelToCoords(pixel, view);
//    }
//
//    void Window::setJoystickThreshold(float threshold) {
//        getRenderWindow().setJoystickThreshold(threshold);
//    }
//
//    void Window::setKeyRepeatEnabled(bool enabled) {
//        getRenderWindow().setKeyRepeatEnabled(enabled);
//    }
//
//    void Window::setMouseCursorGrabbed(bool grabbed) {
//        getRenderWindow().setMouseCursorGrabbed(grabbed);
//    }
//
//    void Window::setMouseCursorVisible(bool visible) {
//        getRenderWindow().setMouseCursorVisible(visible);
//    }
//
////    void Window::setMouseCursor(const sf::Cursor& cursor) {
////        getRenderWindow().setMouseCursor(cursor);
////    }
//
//    void Window::setVerticalSyncEnabled(bool enabled) {
//        getRenderWindow().setVerticalSyncEnabled(enabled);
//    }
//
//    void Window::setVisible(bool visible) {
//        getRenderWindow().setVisible(visible);
//    }
//
//    void Window::invokeUpdate() {
//
//        updateWin32DarkMode();
//        mouse.setScrollDelta({});   // Reset scroll to zero
//        processWindowEvents();
//        mouse.updatePositionData(sf::Mouse::getPosition(getRenderWindow()));
//        mouse.updateButtonData();
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
//    void Window::invokeRender() {
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
//    void Window::recoverImGuiStacks() {
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
//
//    void RenderWindow::processEvent(SDL_Event* event) {
//        ImGui_ImplSDL2_ProcessEvent(&event);
//    }
//
//    void Window::renderContent() {
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
//
//    void Window::updateWin32DarkMode() {
//        // bool const useWin32ImmersiveDarkMode = b::style::get<bool>("b::window.win32ImmersiveDarkMode"); // TODO: <- Interesting Copilot suggestion
//#ifdef B_OS_WINDOWS
//        if (useWin32ImmersiveDarkMode != m_win32IDMActive) {
//            BOOL useDarkMode = static_cast<BOOL>(useWin32ImmersiveDarkMode);
//            bool const setImmersiveDarkModeSuccess = SUCCEEDED(DwmSetWindowAttribute(
//                    getRenderWindow().getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
//                    &useDarkMode, sizeof(useDarkMode)));
//            m_win32IDMActive = useWin32ImmersiveDarkMode;
//            if (!setImmersiveDarkModeSuccess) {
//                throw std::runtime_error("Failed to set immersive dark mode on b::window");
//            }
//        }
//#endif
//    }
//
//    void Window::processWindowEvents() {
//        sf::Event event {};
//        while (getRenderWindow().pollEvent(event)) {
//            ImGui::SFML::ProcessEvent(getRenderWindow(), event);
//
//            switch (event.type) {
//                case sf::Event::Closed:
//                    if (!dispatchEvent<b::Events::WindowCloseEvent>()) {
//                        getRenderWindow().close();
//                    }
//                    break;
//
//                case sf::Event::Resized:
//                    getRenderWindow().setView(sf::View(sf::FloatRect({ 0, 0 }, b::Vec2(event.size.width, event.size.height))));
//                    dispatchEvent<b::Events::WindowResizeEvent>(event.size.width, event.size.height);
//                    break;
//
//                case sf::Event::LostFocus:
//                    dispatchEvent<b::Events::WindowLostFocusEvent>();
//                    break;
//
//                case sf::Event::GainedFocus:
//                    dispatchEvent<b::Events::WindowGainedFocusEvent>();
//                    break;
//
//                case sf::Event::KeyPressed:
//                    dispatchEvent<b::Events::KeyPressEvent>(event.key);
//                    break;
//
//                case sf::Event::KeyReleased:
//                    dispatchEvent<b::Events::KeyReleaseEvent>(event.key);
//                    break;
//
//                case sf::Event::TextEntered:
//                    dispatchEvent<b::Events::TextEnteredEvent>(event.text);
//                    break;
//
//                case sf::Event::MouseWheelScrolled:
//                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
//                        mouse.setScrollDelta(b::Vec2(0, event.mouseWheelScroll.delta));
//                    }
//                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
//                        mouse.setScrollDelta(b::Vec2(event.mouseWheelScroll.delta, 0));
//                    }
//                    dispatchEvent<b::Events::MouseWheelScrollEvent>(mouse.scrollDelta);
//                    break;
//
//                case sf::Event::MouseButtonPressed:
//                    dispatchEvent<b::Events::MouseButtonPressEvent>(event.mouseButton);
//                    break;
//
//                case sf::Event::MouseButtonReleased:
//                    dispatchEvent<b::Events::MouseButtonReleaseEvent>(event.mouseButton);
//                    break;
//
//                case sf::Event::MouseMoved:
//                    m_mouseMoveEventData.updatePositionData(b::Vec2(event.mouseMove.x, event.mouseMove.y));
//                    dispatchEvent<b::Events::MouseMoveEvent>(m_mouseMoveEventData);
//                    break;
//
//                case sf::Event::MouseEntered:
//                    dispatchEvent<b::Events::MouseEnteredWindowEvent>();
//                    break;
//
//                case sf::Event::MouseLeft:
//                    dispatchEvent<b::Events::MouseLeftWindowEvent>();
//                    break;
//
//                case sf::Event::JoystickButtonPressed:
//                    dispatchEvent<b::Events::JoystickButtonPressEvent>(event.joystickButton);
//                    break;
//
//                case sf::Event::JoystickButtonReleased:
//                    dispatchEvent<b::Events::JoystickButtonReleaseEvent>(event.joystickButton);
//                    break;
//
//                case sf::Event::JoystickMoved:
//                    dispatchEvent<b::Events::JoystickMoveEvent>(event.joystickMove);
//                    break;
//
//                case sf::Event::JoystickConnected:
//                    dispatchEvent<b::Events::JoystickConnectEvent>(event.joystickConnect);
//                    break;
//
//                case sf::Event::JoystickDisconnected:
//                    dispatchEvent<b::Events::JoystickDisconnectEvent>(event.joystickConnect);
//                    break;
//
//                case sf::Event::TouchBegan:
//                    dispatchEvent<b::Events::TouchBeginEvent>(event.touch);
//                    break;
//
//                case sf::Event::TouchMoved:
//                    dispatchEvent<b::Events::TouchMoveEvent>(event.touch);
//                    break;
//
//                case sf::Event::TouchEnded:
//                    dispatchEvent<b::Events::TouchEndEvent>(event.touch);
//                    break;
//
//                case sf::Event::SensorChanged:
//                    dispatchEvent<b::Events::SensorChangeEvent>(event.sensor);
//                    break;
//
//                default:
//                    break;
//            }
//
//        }
//    }
//
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
//
//}