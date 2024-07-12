#pragma once

#include "../command/command.hpp"
#include "../details/constants.hpp"

#include <memory>

namespace Lavash {
class Node {
public:
    virtual int Execute() = 0;

    virtual ~Node() = default;
};

using NodePtr = std::shared_ptr<Node>;

class CommandNode : public Node {
public:
    int Execute() override;

    void SetCommand(Command &&command) {
        command_ = command;
    }

private:
    Command command_;
};

class BinaryOpNode : public Node {
public:
    explicit BinaryOpNode(BinaryOpType type, NodePtr left = nullptr,
                          NodePtr right = nullptr);

    int Execute() override;

private:
    BinaryOpType type_;
    NodePtr left_;
    NodePtr right_;
};
} // namespace Lavash
