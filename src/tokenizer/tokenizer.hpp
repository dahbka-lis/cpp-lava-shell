#pragma once

#include "../details/constants.hpp"
#include "../reader/reader.hpp"

#include <istream>
#include <variant>

namespace Lavash {
struct Token {
    bool operator==(const Token &other) const;

    TokenType type;
    std::string data;
};

class Tokenizer {
public:
    explicit Tokenizer(std::istream *istream = nullptr);

    bool HasNext() const;

    void Next();

    Token GetToken() const;

    Token GetNextToken();

private:
    Reader reader_;
    Token current_token_;
};
} // namespace Lavash
