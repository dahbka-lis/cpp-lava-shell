#include "tokenizer.hpp"

#include <stdexcept>

using Lavash::Details::TokenType;

namespace Lavash {
bool Token::operator==(const Token &other) const {
    return type == other.type && data == other.data;
}

Tokenizer::Tokenizer(std::istream *istream) {
    reader_.SetStream(istream);
    Next();
}

bool Tokenizer::HasNext() const {
    return !reader_.IsEnd();
}

Token Tokenizer::GetNextToken() {
    Next();
    return GetToken();
}

void Tokenizer::Next() {
    if (!HasNext()) {
        current_token_ = {TokenType::END};
        return;
    }

    auto word = reader_.ReadWord();

    if (word == "&&") {
        current_token_ = {TokenType::AND};
    } else if (word == "||") {
        current_token_ = {TokenType::OR};
    } else if (word[0] == '<') {
        current_token_ = {TokenType::INPUT};
    } else if (word[0] == '>') {
        current_token_ = {TokenType::OUTPUT};
    } else if (word[0] == '(') {
        current_token_ = {TokenType::L_PAREN};
    } else if (word[0] == ')') {
        current_token_ = {TokenType::R_PAREN};
    } else if (word[0] == '|') {
        current_token_ = {TokenType::PIPE};
    } else {
        current_token_ = {TokenType::ARG, word};
    }
}

Token Tokenizer::GetToken() const {
    return current_token_;
}
} // namespace Lavash
