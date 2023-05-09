#pragma once

#include "battery/graphics/unit_property.hpp"

namespace b {

    class property_pack {
    public:
        property_pack() = default;
        property_pack(const unit_property& property_value) { operator=(property_value); }
        property_pack(const std::array<unit_property, 2>& property_value) { operator=(property_value); }

        std::string string() const;

        property_pack& operator=(const unit_property& property_value);
        property_pack& operator=(const std::array<unit_property, 2>& property_value);

        [[nodiscard]] std::vector<unit_property> get_properties() const { return properties; }

    private:
        std::vector<unit_property> properties;
    };

}