#pragma once

#include "instruction.hpp"
#include <vector>
#include <unordered_map>
#include <string>

enum class InterpretResult
{
    OK,
    COMPILE_ERROR,
    RUNTIME_ERROR,
};

struct Tape
{
    std::vector<char> values;
    size_t ptr;
    Tape(): values(std::vector<char>(30000)), ptr(0) {};
};

class VM
{
private:
    Instructions& instructions;
    unsigned ip;
    std::unordered_map<std::string, Tape> tapes;

    void runtimeError(const char* format, ...);
public:
    VM(Instructions& i): instructions(i), ip(0), tapes(std::unordered_map<std::string, Tape>()) {};
    InterpretResult interpret(const std::string& source);
    InterpretResult run();
};
