#include "lavash.hpp"

#include "../parser/parser.hpp"

#include <iostream>

namespace Lavash {
void Run(char **env = nullptr) {
    std::cout << "[LavaSH] > ";

    Tokenizer tokenizer;
    Parser parser{&tokenizer, env};

    auto node = parser.Parse();
    node->Execute();
}
} // namespace Lavash
