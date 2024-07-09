cmake_minimum_required(VERSION 3.28)
project(cpp_lava_shell)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(cpp_lava_shell main.cpp
        src/reader/reader.cpp
        src/parser/parser.cpp
        src/command/command.cpp
        src/lavash/lavash.cpp
        src/tokenizer/tokenizer.cpp)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(test_tokenizer tests/test_tokenizer.cpp src/tokenizer/tokenizer.cpp src/reader/reader.cpp)
target_link_libraries(test_tokenizer PRIVATE Catch2::Catch2WithMain)

add_executable(test_reader tests/test_reader.cpp src/reader/reader.cpp)
target_link_libraries(test_reader PRIVATE Catch2::Catch2WithMain)
