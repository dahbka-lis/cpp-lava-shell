#pragma once

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

    static void TrimSpaces(std::string &str);

private:
    std::stringstream ss_;
    std::string next_word_;
};
} // namespace Lavash
