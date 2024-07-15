#include "lavash.hpp"

#include "../parser/parser.hpp"

#include <iostream>

namespace Lavash {
void Lavash::Run() {
    std::cout << "[LavaSH] > ";

    Tokenizer tokenizer;
    Parser parser{&tokenizer};

    auto node = parser.Parse();
    node->Execute();
}
} // namespace Lavash
