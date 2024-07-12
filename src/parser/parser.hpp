#pragma once

#include "../nodes/nodes.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <string>

namespace Lavash {
class Parser {
public:
    explicit Parser(Tokenizer *tokenizer);

    NodePtr Parse();

private:
    Token CurrentToken() const;

    void ConsumeToken();

    void Eat(TokenType type);

    NodePtr ParseExpression();

    NodePtr ParseTerm();

    NodePtr ParseFactor();

    Command ParseCommand();

    Subcommand ParseSubcommand();

    bool IsCommandToken() const;

    bool IsPipeToken() const;

private:
    Tokenizer *tokenizer_;
};

} // namespace Lavash
