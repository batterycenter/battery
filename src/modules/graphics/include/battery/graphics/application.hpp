#pragma once

#include "battery/core/console_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/context.hpp"
#include "battery/python/python.hpp"

#define B_DEF_PY_APPLICATION(T) inline static T& get() { return dynamic_cast<T&>(b::BaseApplication::get()); }

namespace b {

    class BaseApplication : public b::ConsoleApplication {
    public:
        BaseApplication();
        virtual ~BaseApplication() = default;

        static BaseApplication & get();

        virtual void onSetup() = 0;
        virtual void onUpdate() = 0;
        virtual void onCleanup() = 0;

        template<b::derived_from<b::BaseWindow> T>
        void attachWindow(T** windowPtr) {
            m_windows.emplace_back(std::make_shared<T>());
            *windowPtr = dynamic_cast<T*>(m_windows.back().get());
            (*windowPtr)->onAttach();
        }

        template<b::derived_from<b::BaseWindow> T>
        void detachWindow(T** windowPtr) {
            (*windowPtr)->onDetach();
            auto index = std::distance(m_windows.begin(), std::find_if(m_windows.begin(), m_windows.end(), [&](auto& window) {
                return window.get() == *windowPtr;
            }));
            if (index == m_windows.size()) {
                throw std::runtime_error("Failed to detach window: No such window is registered");
            }
            m_windows.erase(m_windows.begin() + index);
            *windowPtr = nullptr;
        }

        void detachWindows();

        std::vector<std::shared_ptr<b::BaseWindow>>& windows();

    private:
        void onConsoleSetup() final override;
        void onConsoleUpdate() final override;
        void onConsoleCleanup() final override;

        b::py::scoped_interpreter guard{};
        std::vector<std::shared_ptr<b::BaseWindow>> m_windows;
    };

    template<b::derived_from<b::PyContext> T, b::template_string_literal ContextName>
    class PyApplication : public BaseApplication {
    public:
        T m_context;

        PyApplication() = default;

        void definePythonClasses(b::py::module& module) {
            b::define_python_types(module);

            m_context.defineParentPythonClass(module);
            m_context.definePythonClass(module);
            module.attr(ContextName.value) = &m_context;

            for (auto& window : windows()) {
                window->definePythonClass(module);
            }
        }
    };

}