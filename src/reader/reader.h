#pragma once

#include <regex>
#include <sstream>
#include <string>

namespace Lavash {
class Reader {
public:
    void SetStream(std::istream *stream = nullptr);

    std::string ReadWord();

    bool IsEnd() const;

private:
    void ConfigureNextWord();

    void SkipSpaces();

    bool IsInputOutput();

    bool IsBracket();

    bool IsSpecialSymbol();

    static void TrimSpaces(std::string &str);

private:
    std::stringstream ss_;
    std::string next_word_;
    std::regex bracket_pattern_{"^([^()]*(\\(|\\))+)$"};
};
} // namespace Lavash
