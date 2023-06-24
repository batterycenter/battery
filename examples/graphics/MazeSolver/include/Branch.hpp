#pragma once

#include "pch.hpp"

struct Branch {
    bool m_isDeadEnd = false;
    std::vector<b::vec2> m_path;
    std::shared_ptr<Branch> m_parent;
    std::vector<std::shared_ptr<Branch>> m_children;
};