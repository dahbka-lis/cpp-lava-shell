#pragma once

#include <string>

namespace Lavash {
class Parser {
public:
    Parser() = delete;

    static std::string ParseLine(const std::string &line);
};

} // namespace Lavash
