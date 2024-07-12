#include "parser.hpp"

#include <cassert>
#include <iostream>

namespace Lavash {
Parser::Parser(Tokenizer *tokenizer) : tokenizer_(tokenizer) {
    assert(tokenizer != nullptr && "Tokenizer should not be null");
}

Token Parser::CurrentToken() const {
    return tokenizer_->GetToken();
}

void Parser::ConsumeToken() {
    tokenizer_->Next();
}

void Parser::Eat(TokenType type) {
    if (CurrentToken().type != type) {
        throw std::runtime_error("Type mismatch");
    }

    ConsumeToken();
}

NodePtr Parser::Parse() {
    return ParseExpression();
}

NodePtr Parser::ParseExpression() {
    auto node = ParseTerm();

    while (CurrentToken().type == TokenType::OR) {
        Eat(TokenType::OR);
        node =
            std::make_shared<BinaryOpNode>(BinaryOpType::OR, node, ParseTerm());
    }

    return node;
}

NodePtr Parser::ParseTerm() {
    auto node = ParseFactor();

    while (CurrentToken().type == TokenType::AND) {
        Eat(TokenType::AND);
        node = std::make_shared<BinaryOpNode>(BinaryOpType::AND, node,
                                              ParseFactor());
    }

    return node;
}

NodePtr Parser::ParseFactor() {
    if (CurrentToken().type == TokenType::L_PAREN) {
        Eat(TokenType::L_PAREN);
        auto node = ParseExpression();
        Eat(TokenType::R_PAREN);
        return node;
    }

    auto node = std::make_shared<CommandNode>();
    node->SetCommand(ParseCommand());
    return node;
}

Command Parser::ParseCommand() {
    Command res;

    while (IsCommandToken()) {
        res.AddSubcommand(ParseSubcommand());

        if (IsPipeToken()) {
            Eat(TokenType::PIPE);
        }
    }

    return res;
}

Subcommand Parser::ParseSubcommand() {
    Subcommand res;

    while (IsCommandToken() && !IsPipeToken()) {
        if (CurrentToken().type == TokenType::INPUT) {
            Eat(TokenType::INPUT);
            res.SetInputFile(CurrentToken().data);
            Eat(TokenType::ARG);
        } else if (CurrentToken().type == TokenType::OUTPUT) {
            Eat(TokenType::OUTPUT);
            res.SetOutputFile(CurrentToken().data);
            Eat(TokenType::ARG);
        } else {
            res.AddArg(CurrentToken().data);
            Eat(TokenType::ARG);
        }
    }

    return res;
}

bool Parser::IsCommandToken() const {
    auto cur = CurrentToken().type;
    return cur == TokenType::ARG || cur == TokenType::PIPE ||
           cur == TokenType::INPUT || cur == TokenType::OUTPUT;
}

bool Parser::IsPipeToken() const {
    return CurrentToken().type == TokenType::PIPE;
}
} // namespace Lavash
