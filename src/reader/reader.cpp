#include "reader.h"

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
    ss_ >> next_word_;
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
