
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    BaseApplication::BaseApplication() {
        setRequestedFramerate(60.0);

    }

    BaseApplication & BaseApplication::get() {
        return dynamic_cast<BaseApplication&>(b::ConsoleApplication::get());
    }

    void BaseApplication::detachWindows() {
        for (auto& window : m_windows) {
            window->onDetach();
        }
        m_windows.clear();
    }

    std::vector<std::shared_ptr<b::Window>>& BaseApplication::windows() {
        return m_windows;
    }

    void BaseApplication::onConsoleSetup() {
        onSetup();
    }

    void BaseApplication::onConsoleUpdate() {
        onUpdate();
        for (auto& window : m_windows) {
            if (window->isOpen()) {
                window->framecount = m_framecount;
                window->frametime = m_frametime;
                window->framerate = m_framerate;
                window->invoke_update();
            }
        }
    }

    void BaseApplication::onConsoleCleanup() {
        onCleanup();
        detachWindows();
        ImGui::SFML::Shutdown();
    }

} // namespace b