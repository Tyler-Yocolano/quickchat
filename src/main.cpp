#include "instruction.hpp"
#include "vm.hpp"
#include <cstdarg>
#include <iostream>
#include <fstream>

static void repl(VM& vm)
{
    std::string line;

    while (true)
    {
        std::cout << "quickchat> ";
        std::getline(std::cin, line);

        vm.interpret(line + "\r\n");

        if (std::cin.eof())
        {
            std::cout << std::endl;
            break;
        }
    }
}

static std::string readFile(const std::string& path)
{
    std::ifstream t(path);
    if (!t.is_open())
    {
        std::cout << "Failed to open " << path << std::endl;
        exit(74);
    }
    std::string str;

    t.seekg(0, std::ios::end);
    try
    {
        str.reserve(t.tellg());
    }
    catch(std::length_error err)
    {
        std::cout << "File is too long" << std::endl;
        exit(74);
    }
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());

    return str;
}

static void runFile(VM& vm, const std::string& path)
{
    std::string source = readFile(path);
    InterpretResult result = vm.interpret(source);

    switch (result)
    {
        case InterpretResult::COMPILE_ERROR: exit(65);
        case InterpretResult::OK: break;
        case InterpretResult::RUNTIME_ERROR: exit(70);
    }
}

int main(int argc, const char* argv[])
{
    auto instructions = Instructions();
    auto vm = VM(instructions);

    if (argc == 1)
    {
        repl(vm);
    }
    else if (argc == 2)
    {
        runFile(vm, argv[1]);
    }
    else
    {
        std::cerr << "Usage: quickchat [path]" << std::endl;
        exit(64);
    }
}
