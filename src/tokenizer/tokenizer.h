#pragma once

#include "../reader/reader.h"

#include <istream>
#include <variant>

namespace Lavash {
struct ArgToken {
    bool operator==(const ArgToken &other) const;

    std::string arg;
};

struct PipeToken {
    bool operator==(const PipeToken &other) const;
};

struct InputToken {
    bool operator==(const InputToken &other) const;
};

struct OutputToken {
    bool operator==(const OutputToken &other) const;
};

struct LogicAndToken {
    bool operator==(const LogicAndToken &other) const;
};

struct LogicOrToken {
    bool operator==(const LogicOrToken &other) const;
};

enum class BracketToken { OPEN, CLOSE };

using Token = std::variant<ArgToken, PipeToken, InputToken, OutputToken,
                           LogicAndToken, LogicOrToken, BracketToken>;

class Tokenizer {
public:
    explicit Tokenizer(std::istream *istream);

    bool HasNext() const;

    Token GetNextToken();

private:
    Reader reader_;
};
} // namespace Lavash
