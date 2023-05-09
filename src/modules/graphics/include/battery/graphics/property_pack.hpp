#pragma once

#include "battery/graphics/unit_property.hpp"

namespace b {

    class property_pack {
    public:
        property_pack() = default;
        property_pack(const unit_property& property_value);
        property_pack(const std::pair<unit_property, unit_property>& property_value);
        property_pack(const unit_property& property_x, const unit_property& property_y);
        property_pack(const std::vector<unit_property>& property_value);

        std::string string() const;

        [[nodiscard]] std::vector<unit_property> get_properties() const { return properties; }

    private:
        std::vector<unit_property> properties;
    };

}