#include "parser.hpp"
#include <iostream>
#include <cstdarg>
#include <optional>
#include <algorithm>

//#define DEBUG_PRINT_CODE

static int lastTape = 0;

Parser::Parser(const std::string& source, Instructions& instructions)
    : current(Token(TokenType::_EOF, source, 0)),
    previous(Token(TokenType::_EOF, source, 0)),
    lexer(Lexer(source)),
    instructions(instructions),
    loopLevel(0),
    deleted(std::unordered_set<std::string>()),
    hadError(false),
    panicMode(false)
{
    advance();
}

void Parser::errorAt(const Token& token, const std::string& message)
{
    if (panicMode) return;
    panicMode = true;
    std::cerr << "[line " << token.line << "] Error";

    if (token.type == TokenType::_EOF)
    {
        std::cerr << " at end";
    }
    else if (token.type == TokenType::_ERROR)
    {
        // NOTHING
    }
    else if (token.type == TokenType::NEW_LINE)
    {
        std::cerr << " at end of line";
    }
    else
    {
        std::cerr << " at '" << token.text << "'";
    }

    std::cerr << ": " << message << std::endl;
    hadError = true;
}

void Parser::error(const std::string& message)
{
    errorAt(previous, message);
}

void Parser::errorAtCurrent(const std::string& message)
{
    errorAt(current, message);
}

void Parser::synchronize()
{
    panicMode = false;

    while (current.type != TokenType::_EOF)
    {
        if (previous.type == TokenType::NEW_LINE) return;
        advance();
    }
}

void Parser::advance()
{
    previous = current;

    while (true)
    {
        current = lexer.scanToken();
        if (current.type != TokenType::_ERROR) break;

        errorAtCurrent(std::string(current.text));
    }
}

void Parser::consume(enum TokenType type, const std::string& message)
{
    if (current.type == type)
    {
        advance();
        return;
    }
    else
    {
        errorAtCurrent(message);
    }
}

bool Parser::compile()
{
    while (!match(TokenType::_EOF))
    {
        line();
    }

    end();
    return !hadError;
}

void Parser::line()
{
    if (match(TokenType::IDENTIFIER))
    {
        auto name = std::string(previous.text);
        auto capsName = std::string(previous.text);
        std::transform(capsName.begin(), capsName.end(), capsName.begin(), ::toupper);

        auto hasBeenDeleted = std::find(deleted.begin(), deleted.end(), capsName) != deleted.end();

        if (match(TokenType::COLON))
        {
            if (hasBeenDeleted) error(name + " has left the match.");
            consume(TokenType::SINGLE_SPACE, "Expected '<NAME>: <COMMAND>'.");

            if (name != capsName)
            {
                error("Name must be in ALL CAPS.");
            }

            auto idx = instructions.findName(capsName);
            if (!idx.has_value())
            {
                error(name + " has not joined the match.");
            }
            else
            {
                lastTape = idx.value();
                advance();
                switch(previous.type)
                {
                    case TokenType::NO_PROBLEM:
                        emitBytes(OpCode::DECATPTR, idx.value()); 
                        break;
                    case TokenType::DEFENDING:
                        emitBytes(OpCode::DECPTR, idx.value());
                        break;
                    case TokenType::I_GOT_IT:
                        emitBytes(OpCode::INCPTR, idx.value());
                        break;
                    case TokenType::NICE_SHOT:
                        emitBytes(OpCode::INCATPTR, idx.value());
                        break;
                    case TokenType::CALCULATED:
                        emitBytes(OpCode::OUTPUT, idx.value());
                        break;
                    case TokenType::GREAT_PASS:
                        emitBytes(OpCode::COPY_FROM, idx.value());
                        break;
                    case TokenType::TAKE_THE_SHOT:
                    {
                        auto loopStart = instructions.codeCount();
                        loopLevel++;
                        auto currentLoop = loopLevel;
                        auto thenJump = emitJump(idx.value());
                        endLine();
                        while (loopLevel >= currentLoop)
                        {
                            if (match(TokenType::_EOF))
                            {
                                error("'What a save!' required for previous 'Take the shot!'");
                                return;
                            }
                            line();
                        }
                        if (idx.value() != lastTape)
                        {
                            error("Loop must end with the same player: " + name);
                            return;
                        }
                        emitLoop(idx.value(), loopStart);
                        instructions.patchJump(idx.value(), thenJump);
                        if (panicMode) synchronize();
                        return;
                    }
                    case TokenType::INCOMING:
                        emitBytes(OpCode::INPUT, idx.value());
                        break;
                    case TokenType::WHAT_A_SAVE:
                        loopLevel--;
                        break;
                    default:
                        error("Unrecognized command.");
                        break;
                }
            }
        }
        else
        {
            consume(TokenType::SINGLE_SPACE, "Expected '<Name> <JOINED/LEFT>'");
            if (match(TokenType::JOINED))
            {
                auto idx = instructions.defineName(std::string(capsName), previous.line);
                if (!idx.has_value())
                {
                    if (hasBeenDeleted)
                    {
                        emitBytes(OpCode::DEFINE_NAME, idx.has_value());
                    }
                    else
                    {
                        error(name + " has already joined the match.");
                    }
                }
            }
            else if (match(TokenType::LEFT))
            {
                if (hasBeenDeleted) error(name + " has left the match.");
                auto idx = instructions.findName(capsName);
                if (idx.has_value())
                {
                    deleted.insert(capsName);
                    emitBytes(OpCode::DELETE_NAME, idx.value());
                }
                else
                {
                    error(name + " has not joined the match.");
                }
            }
            else
            {
                error("Expected '<Name> <JOINED/LEFT>' or '<NAME>: <COMMAND>'.");
            }
        }
        endLine();
    }
    else
    {
        error("Expected a name.");
    }

    if (panicMode) synchronize();
}

void Parser::endLine()
{
    if (!match(TokenType::_EOF))
        consume(TokenType::NEW_LINE, "Expected new line after command.");
}

int Parser::emitJump(int tape)
{
    emitByte(OpCode::BEGIN);
    int thenJump = instructions.codeCount();
    emitByte(tape);
    emitByte(0xFF);
    emitByte(0xFF);

    return thenJump;
}

void Parser::emitLoop(int tape, int loopStart)
{
    emitBytes(OpCode::END, tape);

    int offset = instructions.codeCount() - loopStart + 2;
    if (offset > UINT16_MAX) error("Loop body too large.");

    emitByte((offset >> 8) & 0xFF);
    emitByte(offset & 0xFF);
}

void Parser::end()
{
    /*
    if (lastTape >= 0)
    {
        emitBytes(OpCode::OUTPUT, lastTape);
    }
    */
#ifdef DEBUG_PRINT_CODE
    if (!hadError)
    {
        instructions.disassemble("code");
    }
#endif
}

bool Parser::match(enum TokenType type)
{
    if (current.type != type) return false;
    advance();
    return true;
}

void Parser::emitByte(uint8_t byte)
{
    instructions.write(byte, previous.line);
}

void Parser::emitByte(OpCode opcode)
{
    emitByte(static_cast<uint8_t>(opcode));
}

void Parser::emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1); emitByte(byte2);
}

void Parser::emitBytes(OpCode opcode, uint8_t byte)
{
    emitByte(opcode); emitByte(byte);
}
