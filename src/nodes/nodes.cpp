#include "nodes.hpp"

#include "../details/is_type_node.h"

#include <utility>

using namespace Lavash::Details;

namespace Lavash {
int CommandNode::Execute() {
    return command_.Execute();
}

BinaryOpNode::BinaryOpNode(BinaryOpType type, NodePtr left, NodePtr right)
    : type_(type), left_(std::move(left)), right_(std::move(right)) {
}

int Lavash::BinaryOpNode::Execute() {
    if (type_ == BinaryOpType::AND) {
        return SelectAnd();
    } else if (type_ == BinaryOpType::OR) {
        return SelectOr();
    }

    return 0;
}

int BinaryOpNode::SelectOr() {
    if (IsTypeNode<CommandNode>(left_)) {
        if (IsTypeNode<CommandNode>(right_)) {
            return ExecuteOr(left_, right_);
        }

        auto right_ptr = dynamic_cast<BinaryOpNode *>(right_.get());
        if (right_ptr && right_ptr->type_ == BinaryOpType::OR) {
            return ExecuteOr(left_, right_);
        }

        return ExecuteOr(right_, left_);
    }

    if (IsTypeNode<BinaryOpNode>(left_)) {
        auto right_ptr = dynamic_cast<BinaryOpNode *>(right_.get());
        if (right_ptr && right_ptr->type_ == BinaryOpType::AND) {
            return ExecuteOr(right_, left_);
        }

        if (dynamic_cast<ParenthesesNode *>(right_.get())) {
            return ExecuteOr(right_, left_);
        }

        return ExecuteOr(left_, right_);
    }

    return ExecuteOr(left_, right_);
}

int BinaryOpNode::SelectAnd() {
    if (IsTypeNode<ParenthesesNode>(right_)) {
        return ExecuteAnd(right_, left_);
    }

    return ExecuteAnd(left_, right_);
}

int BinaryOpNode::ExecuteOr(const NodePtr &first, const NodePtr &second) {
    if (first == nullptr) {
        return 0;
    }

    auto code = first->Execute();
    if (code == 0 || second == nullptr) {
        return code;
    }

    return second->Execute();
}

int BinaryOpNode::ExecuteAnd(const NodePtr &first, const NodePtr &second) {
    if (first == nullptr) {
        return 0;
    }

    auto code = first->Execute();
    if (code != 0 || second == nullptr) {
        return code;
    }

    return second->Execute();
}

ParenthesesNode::ParenthesesNode(NodePtr node) : node_(std::move(node)) {
}

int ParenthesesNode::Execute() {
    return node_->Execute();
}
} // namespace Lavash
