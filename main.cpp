#include "src/lavash/lavash.h"

#include <cstdio>
#include <cstdlib>

int main() {
    try {
        Lavash::Lavash().Run();
    } catch (...) {
        perror("Lavash.Run()");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
