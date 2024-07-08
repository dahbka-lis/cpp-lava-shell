#include <catch2/catch_test_macros.hpp>

#include "../src/tokenizer/tokenizer.h"

using namespace Lavash;

TEST_CASE("Simple") {
    Tokenizer null_tokenizer{nullptr};

    REQUIRE(!null_tokenizer.HasNext());
    REQUIRE_THROWS(null_tokenizer.GetNextToken());

    std::stringstream ss{"foo bar"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"foo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"bar"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Arguments") {
    std::stringstream ss{"echo hello world !"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"echo"}});

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
    std::stringstream ss{"echo linux | wc | cat"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"linux"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"cat"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 1") {
    std::stringstream ss{"cat < input.txt > output.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"cat"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{"input.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{"output.txt"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Files 2") {
    std::stringstream ss{
        "echo hello > output.txt | wc < output.txt > output2.txt"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"hello"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{"output2.txt"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());

    std::stringstream err{"echo error >"};
    Tokenizer err_tokenizer{&ss};

    REQUIRE(err_tokenizer.HasNext());
    REQUIRE(err_tokenizer.GetNextToken() == Token{PathToken{"echo"}});

    REQUIRE(err_tokenizer.HasNext());
    REQUIRE(err_tokenizer.GetNextToken() == Token{ArgToken{"error"}});

    REQUIRE(err_tokenizer.HasNext());
    REQUIRE_THROWS(err_tokenizer.GetNextToken());
}

TEST_CASE("Logic") {
    std::stringstream ss{"true && false || true"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"true"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicAndToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"false"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"true"}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("Brackets") {
    std::stringstream ss{"(true || false)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::OPEN}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"true"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"false"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::CLOSE}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}

TEST_CASE("All") {
    std::stringstream ss{"cat file > output.txt || (wc < input.txt | echo)"};
    Tokenizer tokenizer{&ss};

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"cat"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{ArgToken{"file"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{OutputToken{"output.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{LogicOrToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::OPEN}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"wc"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{InputToken{"input.txt"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PipeToken{}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() == Token{PathToken{"echo"}});

    REQUIRE(tokenizer.HasNext());
    REQUIRE(tokenizer.GetNextToken() ==
            Token{BracketToken{BracketToken::CLOSE}});

    REQUIRE(!tokenizer.HasNext());
    REQUIRE_THROWS(tokenizer.GetNextToken());
}
