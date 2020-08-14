#include "lexer.hpp"

const std::string charError = "Unexpected character.";
std::unordered_map<std::string, TokenType> Lexer::tokenMap =
{
    {"No problem.",         TokenType::NO_PROBLEM},
    {"Defending...",        TokenType::DEFENDING},
    {"I got it!",           TokenType::I_GOT_IT},
    {"joined the match",    TokenType::JOINED},
    {"left the match",      TokenType::LEFT},
    {"Nice shot!",          TokenType::NICE_SHOT},
    {"Calculated.",         TokenType::CALCULATED},
    {"Great pass!",         TokenType::GREAT_PASS},
    {"Take the shot!",      TokenType::TAKE_THE_SHOT},
    {"Incoming!",           TokenType::INCOMING},
    {"What a save!",        TokenType::WHAT_A_SAVE},
};

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool isPunctuation(char c)
{
    return (c == '.' || c == '!' || c == ' ');
}

Token Lexer::scanToken()
{  
    start = current;

    if (isAtEnd()) return makeToken(TokenType::_EOF);

    auto c = advance();

    switch (c)
    {
        case ' ': return makeToken(TokenType::SINGLE_SPACE);
        case ':': return makeToken(TokenType::COLON);
        case '\r':
        {
            if (peek() == '\n') advance();
            line++;
            return makeToken(TokenType::NEW_LINE);
        }
        case '\n':
        {
            line++;
            return makeToken(TokenType::NEW_LINE);
        }
    }

    if (isAlpha(c)) return identifierOrKeyphrase();

    return errorToken(charError);
}

bool Lexer::isAtEnd()
{
    return current == source.length();
}

char Lexer::advance()
{
    current++;
    return source[current - 1];
}

char Lexer::peek()
{
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext()
{
    if ((current + 1) >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected)
{
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

Token Lexer::makeToken(enum TokenType type)
{
    auto text = std::string_view(&source[start], current - start);
    return Token(type, text, line);
}

Token Lexer::errorToken(const std::string& message)
{
    return Token(TokenType::_ERROR, message, line);
}

Token Lexer::identifierOrKeyphrase()
{
    bool oneWord = true;
    int keyphrase = 1;
    int id = 1;
    while (isAlpha(source[start + keyphrase])
        || isDigit(source[start + keyphrase])
        || isPunctuation(source[start + keyphrase]))
    {
        if (isPunctuation(source[start + keyphrase])) oneWord = false;
        if (oneWord) id++;
        keyphrase++;
    }

    auto search = tokenMap.find(source.substr(start, keyphrase));
    if (search != tokenMap.end())
    {
        current += keyphrase - 1;
        return makeToken(search->second);
    }
    else
    {
        current += id - 1;
        return makeToken(TokenType::IDENTIFIER);
    }
}
