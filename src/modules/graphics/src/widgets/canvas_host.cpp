
#include "battery/graphics/widgets/canvas_host.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/application.hpp"

namespace b::widgets {

    void canvas_host::operator()() {
        b::log::core::warn("b::widgets::canvas_host::operator()() was called without a canvas. "
                           "This will have no effect. Please use the widget-specific operator() and pass a "
                           "callback function, which will be called with a reference to a canvas to draw onto.");
    }

    void canvas_host::operator()(const std::function<void(b::Canvas&)>& renderCallback) {
        basePushStyle();
        baseSetCursorPositionToMinBB();

        auto desiredSize = baseGetBBSize();
        if (desiredSize.x > 0 && desiredSize.y > 0) {
            if (b::Vec2(m_canvas.getSize()) != desiredSize) {
                if (!m_canvas.create(desiredSize)) {
                    b::log::core::error("Failed to create b::Canvas copy of size {}x{}",
                                        desiredSize.x, desiredSize.y);
                }
                m_canvas.setView(sf::View(sf::FloatRect(b::Vec2(0, desiredSize.y), b::Vec2(desiredSize.x, -desiredSize.y))));
            }

            if (b::Vec2(m_canvas.getSize()) == desiredSize) {   // If it is not the correct size,
                renderCallback(m_canvas);                       // something went wrong, thus not render anything
                m_canvas.display();
                ImGui::Image(m_canvas.getTexture());

                hovered = ImGui::IsItemHovered();
                actualPosition = ImGui::GetItemRectMin();
                actualSize = ImGui::GetItemRectSize();

                if (hovered) {
                    auto mouse = b::Application::get().getCurrentlyUpdatingWindow().mouse;
                    m_canvas.mouse = mouse;
                    m_canvas.mouse.pos -= actualPosition;
                    m_canvas.mouse.posPrevious -= actualPosition;
                }
                else {
                    m_canvas.mouse.freezeData();
                }
            }
        }
        else {
            hovered = false;
        }

        basePopStyle();
    }

} // namespace b::widgets