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
} // namespace Lavash::Details
