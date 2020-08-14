#pragma once

#include <string>
#include <unordered_map>

enum class TokenType
{
    // Line
    IDENTIFIER,     //00 
    COLON,          //01 
    SINGLE_SPACE,   //02 Required after colon.
    NEW_LINE,       //03 Required before new chat line.

    //Key phrases
    JOINED,         //04 Make a new Tape
    I_GOT_IT ,      //05 Increment Pointer
    DEFENDING,      //06 Decrement Pointer
    NICE_SHOT,      //07 Increment @ Pointer
    NO_PROBLEM,      //08 Decrement @ Pointer
    CALCULATED,           //09 Output byte
    GREAT_PASS,            //10 Copy value from Ptr pos of tape idx of current ptr value.
    INCOMING,         //11 Input byte
    TAKE_THE_SHOT,  //12 Begin Loop
    WHAT_A_SAVE,    //13 End Loop
    LEFT,           //14 Close / Delete Tape

    // Other
    _ERROR,         //15
    _EOF,           //16
};

struct Token
{
    enum TokenType type;
    std::string_view text;
    int line;

    Token(enum TokenType type, std::string_view text, int line)
        : type(type), text(text), line(line) {};
};

class Lexer
{
private:
    std::string source;
    int start;
    int current;
    int line;

    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);

    Token makeToken(enum TokenType type);
    Token errorToken(const std::string& message);

    static std::unordered_map<std::string, TokenType> tokenMap;
    Token identifierOrKeyphrase();
public:
    Lexer(const std::string& source)
        : source(source), start(0), current(0), line(1) {};
    Token scanToken();
};
