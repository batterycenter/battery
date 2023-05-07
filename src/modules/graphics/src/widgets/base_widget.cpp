
#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    base_widget::base_widget(std::string name) : name(std::move(name)) {
        new_id();
    }

    std::string base_widget::get_identifier() const {
        return name + "##batteryui" + std::to_string(id);
    }

    void base_widget::new_id() {
        id = static_id_counter;
        static_id_counter++;
    }

}