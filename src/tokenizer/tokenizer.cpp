#include "tokenizer.h"

#include <stdexcept>

namespace Lavash {
bool ArgToken::operator==(const ArgToken &other) const {
    return arg == other.arg;
}

bool PipeToken::operator==(const PipeToken &other) const {
    return true;
}

bool InputToken::operator==(const InputToken &other) const {
    return true;
}

bool OutputToken::operator==(const OutputToken &other) const {
    return true;
}

bool LogicAndToken::operator==(const LogicAndToken &other) const {
    return true;
}

bool LogicOrToken::operator==(const LogicOrToken &other) const {
    return true;
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
        return Token{LogicAndToken{}};
    } else if (hash_word == hash("||")) {
        return Token{LogicOrToken{}};
    } else if (word[0] == '<') {
        return Token{InputToken{}};
    } else if (word[0] == '>') {
        return Token{OutputToken{}};
    } else if (word[0] == '(') {
        return Token{BracketToken{BracketToken::OPEN}};
    } else if (word[0] == ')') {
        return Token{BracketToken{BracketToken::CLOSE}};
    } else if (hash_word == hash("|")) {
        return Token{PipeToken{}};
    } else {
        return Token{ArgToken{word}};
    }
}
} // namespace Lavash
