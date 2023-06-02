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

    template<b::derived_from<b::context> T, string_literal ContextName>
    class application : public basic_application {
    public:
        T m_context;

        void apply_python_types(b::py::module& module) {
            b::define_python_types(module);

            m_context.define_python_types(module);
            module.attr(ContextName.value) = m_context;

            for (auto& window : windows()) {
                window->defineContextPythonTypes(module);
            }
        }

        application() = default;
    };

}