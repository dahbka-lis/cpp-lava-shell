#include <catch2/catch_test_macros.hpp>

#include "../src/reader/reader.h"

using namespace Lavash;

TEST_CASE("Simple") {
    std::istringstream ss{"foo bar"};
    Reader reader{&ss};

    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "foo");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "bar");
    REQUIRE(reader.IsEnd());
}

TEST_CASE("Clear") {
    std::istringstream ss{""};
    Reader reader{&ss};

    REQUIRE(reader.IsEnd());
    REQUIRE(reader.ReadWord().empty());
    REQUIRE(reader.ReadWord().empty());
    REQUIRE(reader.ReadWord().empty());
    REQUIRE(reader.ReadWord().empty());
    REQUIRE(reader.IsEnd());
}

TEST_CASE("Long") {
    std::istringstream ss{"a b c d e f g"};
    Reader reader{&ss};

    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "a");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "b");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "c");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "d");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "e");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "f");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "g");
    REQUIRE(reader.IsEnd());
}

TEST_CASE("Command") {
    std::istringstream ss{"echo hello world | wc > file.txt"};
    Reader reader{&ss};

    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "echo");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "hello");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "world");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "|");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "wc");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == ">");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "file.txt");
    REQUIRE(reader.IsEnd());
}

TEST_CASE("Spaces") {
    std::istringstream ss{"   cat          file.txt     |     echo   "};
    Reader reader{&ss};

    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "cat");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "file.txt");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "|");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "echo");
    REQUIRE(reader.IsEnd());
}

TEST_CASE("Spaces and tabs") {
    std::istringstream ss{"  \t \t  ls  \t  -a   | \t\t  mkdir  \t\t  "};
    Reader reader{&ss};

    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "ls");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "-a");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "|");
    REQUIRE(!reader.IsEnd());

    REQUIRE(reader.ReadWord() == "mkdir");
    REQUIRE(reader.IsEnd());
}
