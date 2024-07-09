#include <catch2/catch_test_macros.hpp>

#include "../src/tokenizer/tokenizer.h"

using namespace Lavash;

TEST_CASE("Simple") {
    std::istringstream ss{"foo bar"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"foo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"bar"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Arguments") {
    std::istringstream ss{"echo hello world !"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"hello"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"world"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"!"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Pipes") {
    std::istringstream ss{"echo linux | wc | cat"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"linux"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"cat"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 1") {
    std::istringstream ss{"cat < input.txt > output.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"cat"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"input.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"output.txt"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 2") {
    std::istringstream ss{
        "echo hello > output.txt | wc < output.txt > output2.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"hello"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"output2.txt"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Logic") {
    std::istringstream ss{"true && false || true"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"true"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicAndToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"false"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"true"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Brackets 1") {
    std::istringstream ss{"(true || false)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::OPEN}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"true"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"false"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::CLOSE}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

// TEST_CASE("Brackets 2") {
//     std::istringstream ss{"((()()))"};
//     Tokenizer tokenizer{&ss};
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::OPEN}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::OPEN}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::OPEN}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::CLOSE}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::OPEN}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::CLOSE}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::CLOSE}});
//
//     REQUIRE(tokenizer.HasNext());
//     REQUIRE(tokenizer.GetNextToken() ==
//             Token{BracketToken{BracketToken::CLOSE}});
//
//     REQUIRE(!tokenizer.HasNext());
//     REQUIRE_THROWS(tokenizer.GetNextToken());
// }

TEST_CASE("All") {
    std::istringstream ss{"cat file > output.txt || (wc <input.txt | echo)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"cat"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"file"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::OPEN}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"input.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::CLOSE}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}
