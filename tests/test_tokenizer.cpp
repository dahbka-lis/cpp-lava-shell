#include <catch2/catch_test_macros.hpp>

#include "../src/tokenizer/tokenizer.hpp"

using namespace Lavash;

TEST_CASE("Simple") {
    std::istringstream ss{"foo bar"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "foo"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "bar"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Arguments") {
    std::istringstream ss{"echo hello world !"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "echo"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "hello"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "world"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "!"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Pipes") {
    std::istringstream ss{"echo linux | wc | cat"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "echo"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "linux"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::PIPE});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "wc"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::PIPE});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "cat"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 1") {
    std::istringstream ss{"cat < input.txt > output.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "cat"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::INPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "input.txt"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OUTPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "output.txt"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 2") {
    std::istringstream ss{
        "echo hello > output.txt | wc < output.txt > output2.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "echo"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "hello"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OUTPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "output.txt"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::PIPE});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "wc"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::INPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "output.txt"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OUTPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "output2.txt"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Logic") {
    std::istringstream ss{"true && false || true"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "true"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::AND});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "false"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OR});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "true"});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Brackets 1") {
    std::istringstream ss{"(true || false)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "true"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OR});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "false"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Brackets 2") {
    std::istringstream ss{"((()()))"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("All") {
    std::istringstream ss{"cat file > output.txt || (wc <input.txt | echo)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "cat"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "file"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OUTPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "output.txt"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::OR});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::L_PAREN});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "wc"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::INPUT});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "input.txt"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::PIPE});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::ARG, "echo"});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{TokenType::R_PAREN});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}
