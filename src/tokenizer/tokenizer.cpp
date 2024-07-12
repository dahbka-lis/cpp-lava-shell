#include "tokenizer.hpp"

#include <stdexcept>

namespace Lavash {
bool Token::operator==(const Token &other) const {
    return type == other.type && data == other.data;
}

Tokenizer::Tokenizer(std::istream *istream) {
    reader_.SetStream(istream);
}

bool Tokenizer::HasNext() const {
    return !reader_.IsEnd();
}

Token Tokenizer::GetNextToken() {
    if (!HasNext()) {
        throw std::runtime_error("No more tokens");
    }

    auto word = reader_.ReadWord();

    std::hash<std::string> hash;
    auto hash_word = hash(word);

    if (hash_word == hash("&&")) {
        return {TokenType::AND};
    } else if (hash_word == hash("||")) {
        return {TokenType::OR};
    } else if (word[0] == '<') {
        return {TokenType::INPUT};
    } else if (word[0] == '>') {
        return {TokenType::OUTPUT};
    } else if (word[0] == '(') {
        return {TokenType::L_PAREN};
    } else if (word[0] == ')') {
        return {TokenType::R_PAREN};
    } else if (word[0] == '|') {
        return {TokenType::PIPE};
    } else {
        return {TokenType::ARG, word};
    }
}
} // namespace Lavash
