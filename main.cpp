#include "src/lavash/lavash.hpp"

#include <cstdio>

int main() {
    for (;;) {
        try {
            Lavash::Lavash::Run();
        } catch (...) {
            perror("Lavash.Run()");
        }
    }
}
