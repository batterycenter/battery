
#include "battery/graphics/property_pack.hpp"

namespace b {

    property_pack::property_pack(const unit_property& property_value) {
        properties.push_back(property_value);
    }

    property_pack::property_pack(const std::pair<unit_property, unit_property>& property_value) {
        properties.push_back(property_value.first);
        properties.push_back(property_value.second);
    }

    property_pack::property_pack(const unit_property& property_x, const unit_property& property_y) {
        properties.push_back(property_x);
        properties.push_back(property_y);
    }

    property_pack::property_pack(const std::vector<unit_property>& property_value) {
        properties = property_value;
    }

    std::string property_pack::string() const {
        std::string result = "[";
        for (auto& property : properties) {
            result += property.string() + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += "]";
        return result;
    }

}