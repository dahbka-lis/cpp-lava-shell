#pragma once

#include <istream>
#include <string>
#include <variant>

namespace Lavash {
struct PathToken {
    bool operator==(const PathToken &other) const;

    std::string path;
};

struct ArgToken {
    bool operator==(const ArgToken &other) const;

    std::string arg;
};

struct PipeToken {
    bool operator==(const PipeToken &other) const;
};

struct InputToken {
    bool operator==(const InputToken &other) const;

    std::string filename;
};

struct OutputToken {
    bool operator==(const OutputToken &other) const;

    std::string filename;
};

struct LogicAndToken {
    bool operator==(LogicAndToken other) const;
};

struct LogicOrToken {
    bool operator==(const LogicOrToken &other) const;
};

enum class BracketToken { OPEN, CLOSE };

using Token =
    std::variant<PathToken, ArgToken, PipeToken, InputToken, OutputToken,
                 LogicAndToken, LogicOrToken, BracketToken>;

class Tokenizer {
public:
    explicit Tokenizer(std::istream *istream);

    bool HasNext() const;

    Token GetNextToken();

private:
    std::istream *istream_;
};
} // namespace Lavash
