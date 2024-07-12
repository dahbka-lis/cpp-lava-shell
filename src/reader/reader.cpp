#include "reader.hpp"

#include <algorithm>
#include <iostream>

namespace Lavash {
void Reader::SetStream(std::istream *stream) {
    std::string line;

    if (stream == nullptr) {
        std::getline(std::cin, line);
    } else {
        std::getline(*stream, line);
    }

    TrimSpaces(line);
    ss_ = std::stringstream{line};

    ConfigureNextWord();
}

std::string Reader::ReadWord() {
    if (IsEnd()) {
        return std::string{};
    }

    std::string word = std::move(next_word_);
    ConfigureNextWord();
    return word;
}

bool Reader::IsEnd() const {
    return ss_.eof() && next_word_.empty();
}

void Reader::ConfigureNextWord() {
    SkipSpaces();

    if (IsSpecialSymbol()) {
        next_word_ = std::string{static_cast<char>(ss_.get())};
        return;
    }

    ss_ >> next_word_;

    if (std::regex_search(next_word_, bracket_pattern_)) {
        auto idx = std::min(next_word_.find('('), next_word_.find(')'));
        auto erase_it = next_word_.begin() + static_cast<long>(idx);

        for (auto it = next_word_.end() - 1; it >= erase_it; --it) {
            ss_.putback(*it);
        }

        if (idx == 0) {
            ConfigureNextWord();
            return;
        }

        next_word_.erase(erase_it, next_word_.end());
    }
}

void Reader::SkipSpaces() {
    while (std::isspace(ss_.peek())) {
        ss_.get();
    }
}

bool Reader::IsInputOutput() {
    auto let = static_cast<char>(ss_.peek());
    return let == '>' || let == '<';
}

bool Reader::IsBracket() {
    auto let = static_cast<char>(ss_.peek());
    return let == '(' || let == ')';
}

bool Reader::IsSpecialSymbol() {
    return IsInputOutput() || IsBracket();
}

void Reader::TrimSpaces(std::string &str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        ++start;
    }

    auto end = str.end();
    do {
        --end;
    } while (std::distance(start, end) >= 0 && std::isspace(*end));

    bool in_space = false;
    auto destination = str.begin();

    for (auto it = start; it != end + 1; ++it) {
        if (std::isspace(*it) && !in_space) {
            *destination++ = ' ';
            in_space = true;
        } else if (!std::isspace(*it)) {
            *destination++ = *it;
            in_space = false;
        }
    }

    str.erase(destination, str.end());
}
} // namespace Lavash
