#pragma once

#include "instruction.hpp"
#include "lexer.hpp"
#include <cstdint>
#include <unordered_set>

class Parser
{
private:
    Token current;
    Token previous;
    Lexer lexer;
    Instructions& instructions;
    int loopLevel;
    std::unordered_set<std::string> deleted;

    void advance();
    void consume(enum TokenType type, const std::string& message);

    void emitByte(uint8_t byte);
    void emitByte(OpCode opcode);
    void emitBytes(uint8_t byte1, uint8_t byte2);
    void emitBytes(OpCode opcode, uint8_t byte);
    int emitJump(int tape);
    void emitLoop(int tape, int loopStart);
    
    bool match(enum TokenType type);

    void line();
    void endLine();

    void end();

    bool hadError;
    bool panicMode;
    void synchronize();

    void errorAt(const Token& token, const std::string& message);
    void errorAtCurrent(const std::string& message);
    void error(const std::string& message);
public:
    Parser(const std::string& source, Instructions& instructions);
    bool compile();
};
