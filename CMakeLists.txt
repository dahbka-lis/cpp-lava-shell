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
