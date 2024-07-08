#pragma once

#include <string>

namespace Lavash {
class Reader {
public:
    Reader() = delete;

    static std::string Read(int32_t fd);

    static std::string ReadLine(int32_t fd);
};
} // namespace Lavash
