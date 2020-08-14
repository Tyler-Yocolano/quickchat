#include "vm.hpp"
#include "parser.hpp"
#include "instruction.hpp"
#include <cstdarg>
#include <iostream>

//#define DEBUG_TRACE_EXECUTION

void VM::runtimeError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    std::cerr << std::endl;
    
    std::cerr << "[line " << instructions.getLineAt(ip - 1) << "] in script" << std::endl;
}

InterpretResult VM::interpret(const std::string& source)
{
    auto parser = Parser(source, instructions);

    if (!parser.compile())
    {
        return InterpretResult::COMPILE_ERROR;
    }

    auto result = run();

    return result;
}

InterpretResult VM::run()
{
    auto readByte = [this]() -> uint8_t
    {
        return this->instructions.getCodeAt(this->ip++);
    };

    while (instructions.hasCodeAt(ip))
    {
#ifdef DEBUG_TRACE_EXECUTION
        instructions.disassembleInstruction(ip);
#endif

        auto instruction = OpCode(readByte());
        switch (instruction)
        {
            case OpCode::BEGIN:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                
                uint16_t offset = (readByte() << 8) | readByte();
                if (tape.values[tape.ptr] == 0) ip += offset;

                break;
            }
            case OpCode::DECATPTR:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                tape.values[tape.ptr] = tape.values[tape.ptr] - 1;

                break;
            }
            case OpCode::DECPTR:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                if (tape.ptr == 0)
                {
                    std::string error = "Attempting to decrement the pointer below 0 on " + name + ".";
                    runtimeError(error.c_str());
                    return InterpretResult::RUNTIME_ERROR;
                }
                else
                {
                    tape.ptr--;
                }
                break;
            }
            case OpCode::DEFINE_NAME:
            {
                const auto& name = instructions.getNameAt(readByte());
                tapes[name] = Tape();
                break;
            }
            case OpCode::DELETE_NAME:
            {
                // Should actually erase the tape or something here.
                const auto& name = instructions.getNameAt(readByte());
                tapes[name] = Tape();
                break;
            }
            case OpCode::END:
            {
                readByte(); // Tape name, not required here.
                uint16_t offset = (readByte() << 8) | readByte();
                ip -= offset;
                break;
            }
            case OpCode::INCATPTR:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                tape.values[tape.ptr] = tape.values[tape.ptr] + 1;

                break;
            }
            case OpCode::INCPTR:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                tape.ptr++;
                break;
            }
            case OpCode::INPUT:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                tape.values[tape.ptr] = getchar();
                break;
            }
            case OpCode::OUTPUT:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                std::cout << tape.values[tape.ptr];
                break;
            }
            case OpCode::COPY_FROM:
            {
                const auto& name = instructions.getNameAt(readByte());
                auto& tape = tapes.at(name);
                auto fromIdx = tape.values[tape.ptr];
                if (fromIdx >= instructions.tapeCount())
                {
                    runtimeError("Attempting to copy a value from a tape that does not exist.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                else
                {
                    auto& fromTape = tapes.at(instructions.getNameAt(fromIdx));
                    tape.values[tape.ptr] = fromTape.values[fromTape.ptr];
                }
                break;
            }
        }
    }
    return InterpretResult::OK;
}
