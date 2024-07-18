#pragma once

#include "../nodes/nodes.hpp"

namespace Lavash::Details {
template <typename T> requires std::is_base_of_v<Node, T>
bool IsTypeNode(const NodePtr &node) {
    return dynamic_cast<T *>(node.get()) != nullptr;
}
}
