
#include "battery/graphics/property_pack.hpp"

namespace b {

    b::string property_pack::string() const {
        b::string result = "[";
        for (auto& property : properties) {
            result += property.string() + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += "]";
        return result;
    }

    property_pack& property_pack::operator=(const unit_property& property_value) {
        properties.clear();
        properties.push_back(property_value);
        return *this;
    }

    property_pack& property_pack::operator=(const std::array<unit_property, 2>& property_value) {
        properties.clear();
        properties.push_back(property_value[0]);
        properties.push_back(property_value[1]);
        return *this;
    }

}