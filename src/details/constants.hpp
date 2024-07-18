#pragma once

namespace Lavash::Details {
enum class BinaryOpType { AND, OR };

enum class TokenType {
    ARG,
    PIPE,
    INPUT,
    OUTPUT,
    AND,
    OR,
    L_PAREN,
    R_PAREN,
    END
};

enum Status {
    SUCCESS = 0,
    FAIL_EXEC = 127,
    FAIL_FILE = 256,
    FAIL_PIPE = 513,
    FAIL_FORK = 1025,
};
} // namespace Lavash::Details
