#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>

enum class OpCode: uint8_t 
{
    INCPTR, 
    DECPTR,
    INCATPTR,
    DECATPTR,
    OUTPUT,
    INPUT,
    BEGIN,
    END,

    DEFINE_NAME,
    DELETE_NAME,
    COPY_FROM,
};

class Instructions
{
private:
    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<std::string> names;

    int tapeInstruction(const std::string& name, int offset);
    int jumpInstruction(const std::string& name, int sign, int offset);
public:
    int getLineAt(int instruction) const { return lines[instruction]; };
    std::string getNameAt(int idx) const { return names[idx]; };
    uint8_t getCodeAt(int offset) const { return code[offset]; };
    bool hasCodeAt(int offset) const { return offset < code.size(); };
    int codeCount() const { return code.size(); };
    int tapeCount() const { return names.size(); };

    std::optional<int> defineName(const std::string& name, int line);
    std::optional<int> findName(const std::string& name) const;

    void disassemble(const std::string& name);
    int disassembleInstructionAt(int offset);

    void write(uint8_t byte, int line);
    void write(OpCode opcode, int line);
    void patchJump(int tape, int offset);

    void clear();
};
