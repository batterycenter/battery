#pragma once

#include <string>

namespace b::uuid {

    std::string v4();
    bool is_valid(const std::string& uuid);

} // namespace b
