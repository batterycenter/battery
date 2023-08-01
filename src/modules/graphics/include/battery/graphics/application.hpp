#pragma once

#include "battery/core/console_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/context.hpp"
#include "battery/python/python.hpp"

#define B_DEF_PY_APPLICATION(T) inline static T& get() { return dynamic_cast<T&>(b::Application::get()); }

namespace b {

    class Application : public b::ConsoleApplication {
    public:
        Application();
        virtual ~Application();

        static Application & get();

        virtual void onSetup() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onExit() = 0;

        template<b::derived_from<b::Window> T>
        void attachWindow(T& windowToAttach) {
            if (windowToAttach.isAttached()) {
                throw std::runtime_error("Failed to attach window: This window is already attached");
            }
            windowToAttach.attach();
            m_windows.emplace_back(windowToAttach);
        }

        template<b::derived_from<b::Window> T>
        void detachWindow(T& windowToDetach) {
            windowToDetach.detach();
            auto index = std::distance(m_windows.begin(), std::find_if(m_windows.begin(), m_windows.end(), [&](b::Window& window) {
                return &window == &windowToDetach;
            }));
            if (index == m_windows.size()) {
                throw std::runtime_error("Failed to detach window: This window was not registered");
            }
            m_windows.erase(m_windows.begin() + index);
        }

        void detachWindows();

        b::Window& getCurrentlyUpdatingWindow();

        std::vector<std::reference_wrapper<b::Window>>& windowRefs();

    private:
        void onConsoleSetup() final override;
        void onConsoleUpdate() final override;
        void onConsoleExit() final override;

        b::py::scoped_interpreter m_guard{};
        std::vector<std::reference_wrapper<b::Window>> m_windows;
        b::Window* m_currentActiveWindow;
    };

    template<b::derived_from<b::PyContext> T, b::template_string_literal ContextName>
    class PyApplication : public Application {
    public:
        T context;

        PyApplication() = default;

        void definePythonClasses(b::py::module& module) {
            b::define_python_types(module);

            context.defineParentPythonClass(module);
            context.definePythonClass(module);
            module.attr(ContextName.value) = &context;

            for (auto& window : windowRefs()) {
                window.get().definePythonBindings(module);
            }
        }
    };

}