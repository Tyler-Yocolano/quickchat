cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(quickchat)

add_executable(quickchat 
    src/instruction.cpp
    src/lexer.cpp
    src/main.cpp
    src/parser.cpp
    src/vm.cpp)