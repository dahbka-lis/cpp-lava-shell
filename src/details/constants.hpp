#pragma once

namespace Lavash {
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
} // namespace Lavash
