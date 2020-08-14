#include "instruction.hpp"
#include <iostream>
#include <algorithm>

void Instructions::write(uint8_t byte, int line)
{
    code.push_back(byte);
    lines.push_back(line);
}

void Instructions::write(OpCode opcode, int line)
{
    write(static_cast<uint8_t>(opcode), line);
}

void Instructions::patchJump(int tape, int offset)
{
    int jump = codeCount() - offset - 3;

    code[static_cast<int>(offset + 1)] = (jump >> 8) & 0xFF;
    code[static_cast<int>(offset + 2)] = jump & 0xFF;
}

std::optional<int> Instructions::defineName(const std::string& name, int line)
{
    if (findName(name).has_value())
    {
        return {};
    }
    else
    {
        names.push_back(name);
        auto result = names.size() - 1;
        write(OpCode::DEFINE_NAME, line);
        write(result, line);
        return result;
    }
}

std::optional<int> Instructions::findName(const std::string& name) const
{
    auto result = std::find(names.begin(), names.end(), name);
    if (result != std::end(names))
    {
        return result - names.begin();
    }
    else
    {
        return {};
    }
}

void Instructions::clear()
{
    code.clear();
    lines.clear();
}

int Instructions::tapeInstruction(const std::string& name, int offset)
{
    std::cout << name << " ";
    auto tape = code[static_cast<int>(offset + 1)];
    std::cout << names[tape] << std::endl;
    return offset + 2;
}

int Instructions::jumpInstruction(const std::string& name, int sign, int offset)
{
    std::cout << name << " ";
    auto tape = code[static_cast<int>(offset + 1)];
    std::cout << names[tape] << " ";
    uint16_t jump = (uint16_t)(code[static_cast<int>(offset + 2)] << 8);
    jump |= code[static_cast<int>(offset + 3)];
    std::cout << offset << " -> " << offset + 4 + sign * jump << std::endl;
    return offset + 4;
}

void Instructions::disassemble(const std::string& name)
{
    std::cout << "== " << name << " ==" << std::endl;
    for (int offset = 0; offset < code.size();)
    {
        offset = disassembleInstructionAt(offset);
    }
}

int Instructions::disassembleInstructionAt(int offset)
{
    std::cout << offset << " ";
    auto currentLine = getLineAt(offset);
    if (offset > 0 && currentLine == getLineAt(offset - 1))
    {
        std::cout << "  | ";
    }
    else
    {
        std::cout << currentLine << " ";
    }

    auto instruction = OpCode(code[offset]);
    switch (instruction)
    {
        case OpCode::BEGIN:
            return jumpInstruction("BEGIN", 1, offset);
        case OpCode::DECATPTR:
            return tapeInstruction("DECATPTR", offset);
        case OpCode::DECPTR:
            return tapeInstruction("DECPTR", offset);
        case OpCode::END:
            return jumpInstruction("END", -1, offset);
        case OpCode::INCATPTR:
            return tapeInstruction("INCATPTR", offset);
        case OpCode::INCPTR:
            return tapeInstruction("INCPTR", offset);
        case OpCode::INPUT:
            return tapeInstruction("INPUT", offset);
        case OpCode::OUTPUT:
            return tapeInstruction("OUTPUT", offset);
        case OpCode::DEFINE_NAME:
            return tapeInstruction("DEFINE_NAME", offset);
        case OpCode::DELETE_NAME:
            return tapeInstruction("DELETE_NAME", offset);
        case OpCode::COPY_FROM:
            return tapeInstruction("COPY_FROM", offset);

        default:
            std::cout << "Unknown opcode: " << code[offset] << std::endl;
            return offset + 1;
    }
}
