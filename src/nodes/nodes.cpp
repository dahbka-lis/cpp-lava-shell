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
    if (type_ == BinaryOpType::AND) {
        return ExecuteAnd();
    } else if (type_ == BinaryOpType::OR) {
        return ExecuteOr();
    }

    return 0;
}

int BinaryOpNode::ExecuteOr() {
    if (dynamic_cast<CommandNode *>(left_.get())) {
        if (dynamic_cast<CommandNode *>(right_.get())) {
            return SelectOr(left_, right_);
        }

        auto right_ptr = dynamic_cast<BinaryOpNode *>(right_.get());
        if (right_ptr && right_ptr->type_ == BinaryOpType::OR) {
            return SelectOr(left_, right_);
        }

        return SelectOr(right_, left_);
    }

    if (dynamic_cast<BinaryOpNode *>(left_.get())) {
        auto right_ptr = dynamic_cast<BinaryOpNode *>(right_.get());
        if (right_ptr && right_ptr->type_ == BinaryOpType::AND) {
            return SelectOr(right_, left_);
        }

        if (dynamic_cast<ParenthesesNode *>(right_.get())) {
            return SelectOr(right_, left_);
        }

        return SelectOr(left_, right_);
    }

    return SelectOr(left_, right_);
}

int BinaryOpNode::ExecuteAnd() {
    if (dynamic_cast<ParenthesesNode *>(right_.get())) {
        return SelectAnd(right_, left_);
    }

    return SelectAnd(left_, right_);
}

int BinaryOpNode::SelectOr(const NodePtr &first, const NodePtr &second) {
    auto code = first->Execute();
    if (code == 0) {
        return code;
    }

    return second->Execute();
}

int BinaryOpNode::SelectAnd(const NodePtr &first, const NodePtr &second) {
    auto code = first->Execute();
    if (code != 0) {
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
