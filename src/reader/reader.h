#pragma once

#include <string>

namespace Lavash {
class Reader {
public:
    Reader() = delete;

    static std::string ReadLine(std::istream *stream = nullptr);

    static void TrimSpaces(std::string &str);

    static std::string ReadTrimmedLine(std::istream *stream = nullptr);
};
} // namespace Lavash
