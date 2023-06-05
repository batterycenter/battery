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

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

        template<typename T>
        void attachWindow(T** windowPtr) {
            m_windows.emplace_back(std::make_shared<T>());
            *windowPtr = dynamic_cast<T*>(m_windows.back().get());
            (*windowPtr)->attach();
        }

        void detachWindows();

        std::vector<std::shared_ptr<b::basic_window>>& windows();

    private:
        void consoleSetup() final override;
        void consoleUpdate() final override;
        void consoleCleanup() final override;

        b::py::scoped_interpreter guard{};
        std::vector<std::shared_ptr<b::basic_window>> m_windows;
    };

    template<b::derived_from<b::PyContext> T, b::template_string_literal ContextName>
    class PyApplication : public BaseApplication {
    public:
        T m_context;

        PyApplication() = default;

        void definePythonClasses(b::py::module& module) {
            b::define_python_types(module);

            T::defineParentPythonClass(module);
            m_context.definePythonClass(module);
            module.attr(ContextName.value) = m_context;

            for (auto& window : windows()) {
                window->definePythonClass(module);
            }
        }
    };

}