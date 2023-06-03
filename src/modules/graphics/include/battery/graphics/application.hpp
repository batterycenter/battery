#pragma once

#include "battery/core/console_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/context.hpp"
#include "battery/python/python.hpp"

namespace b {

    class basic_application : public b::console_application {
    public:
        basic_application();
        virtual ~basic_application() = default;

        static basic_application & get();

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

        template<typename T>
        void attachWindow(T* windowPtr) {
            m_windows.emplace_back(std::make_shared<T>());
            windowPtr = dynamic_cast<T*>(m_windows.back().get());
            windowPtr->attach();
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

    template<b::derived_from<b::py_context> T, b::template_string_literal ContextName>
    class py_application : public basic_application {
    public:
        T m_context;

        py_application() = default;

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