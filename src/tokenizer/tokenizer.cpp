#include "tokenizer.h"

bool Lavash::PathToken::operator==(const Lavash::PathToken &other) const {
    return path == other.path;
}

bool Lavash::ArgToken::operator==(const Lavash::ArgToken &other) const {
    return arg == other.arg;
}

bool Lavash::PipeToken::operator==(const Lavash::PipeToken &other) const {
    return true;
}

bool Lavash::InputToken::operator==(const Lavash::InputToken &other) const {
    return true;
}

bool Lavash::OutputToken::operator==(const Lavash::OutputToken &other) const {
    return true;
}

bool Lavash::LogicAndToken::operator==(const Lavash::PipeToken &other) const {
    return true;
}

bool Lavash::LogicOrToken::operator==(const Lavash::PipeToken &other) const {
    return true;
}

Lavash::Tokenizer::Tokenizer(std::istream *istream) : istream_(istream) {
}

bool Lavash::Tokenizer::HasNext() const {
    return false;
}

Lavash::Token Lavash::Tokenizer::GetNextToken() {
    return Lavash::Token();
}
