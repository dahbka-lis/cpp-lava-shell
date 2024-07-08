#include "reader.h"

#include <algorithm>
#include <iostream>

namespace Lavash {
std::string Reader::ReadLine(std::istream *stream) {
    std::string line;

    if (stream == nullptr) {
        std::getline(std::cin, line);
        return line;
    }

    std::getline(*stream, line);
    return line;
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

std::string Reader::ReadTrimmedLine(std::istream *stream) {
    auto line = ReadLine(stream);
    TrimSpaces(line);
    return line;
}
} // namespace Lavash
