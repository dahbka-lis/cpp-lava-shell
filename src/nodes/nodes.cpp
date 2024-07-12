#include "nodes.hpp"

#include <cassert>
#include <utility>

namespace Lavash {
int CommandNode::Execute() {
    return command_.Execute();
}

BinaryOpNode::BinaryOpNode(BinaryOpType type, NodePtr left, NodePtr right)
    : type_(type), left_(std::move(left)), right_(std::move(right)) {
}

int Lavash::BinaryOpNode::Execute() {
    assert(left_ != nullptr && "Left node should not be null");
    assert(right_ != nullptr && "Right node should not be null");

    auto code = left_->Execute();
    if (type_ == BinaryOpType::AND) {
        if (code != 0)
            return code;

    } else if (type_ == BinaryOpType::OR) {
        if (code == 0)
            return code;
    }

    return right_->Execute();
}
} // namespace Lavash
