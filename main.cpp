#include "src/lavash/lavash.hpp"

#include <cstdio>

int main(int argc, char *argv[], char *env[]) {
    for (;;) {
        try {
            Lavash::Run(env);
        } catch (...) {
            perror("Lavash.Run()");
        }
    }
}
