//
// Created by Ezra Kahn on 5/1/26.
//

#ifndef LEXER_H
#define LEXER_H

#include "iostream"
#include "vector"

enum TokenType
{
    // Literals: "abc" 123 true
    STRING, NUMBER, TRUE, FALSE,

    // Identifiers: varName
    IDENTIFIER,

    // Keywords: if else func var true false
    IF, ELSE, FUNC, VAR,  PRINT,

    // Syntax Chars: ( ) { } ; ,
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURLY, CLOSE_CURLY, SEMICOLON, COMMA,

    // Unary ops (must be single char): ! ~ ^
    BANG, TILDE,

    // Single char ops: + - * / = & % < >
    PLUS, MINUS, STAR, FRONT_SLASH, EQUAL, AMP, PERCENT, LESS_THAN, GREATER_THAN, CARET,

    // Two char ops: += -= *= /= == && %= <= >= != ^=
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, FRONT_SLASH_EQUAL, EQUAL_EQUAL, AMP_AMP,
    PERCENT_EQUAL, LESS_THAN_EQUAL, GREATER_THAN_EQUAL, BANG_EQUAL, CARET_EQUAL,

    INVALID,

    // Token Groups:
    UNARY_TOKEN_GROUP,
    BINARY_TOKEN_GROUP,
    ASSIGNMENT_TOKEN_GROUP,
    SYNTAX_TOKEN_GROUP,
    KEYWORD_TOKEN_GROUP,
    LITERAL_TOKEN_GROUP,
    NO_TOKEN_GROUP,
    ANY_TOKEN_GROUP,
};
// Returns token group for a given token, ex PLUS -> BINARY_TOKEN_GROUP
constexpr TokenType getTokenGroup(const TokenType t)
{
    switch (t)
    {
    case PLUS:
    case MINUS:
    case STAR:
    case FRONT_SLASH:
    case AMP:
    case PERCENT:
    case LESS_THAN:
    case GREATER_THAN:
    case EQUAL_EQUAL:
    case AMP_AMP:
    case PERCENT_EQUAL:
    case LESS_THAN_EQUAL:
    case GREATER_THAN_EQUAL:
    case BANG_EQUAL:
    case CARET:
        return BINARY_TOKEN_GROUP;
    case EQUAL:
    case CARET_EQUAL:
    case PLUS_EQUAL:
    case MINUS_EQUAL:
    case STAR_EQUAL:
    case FRONT_SLASH_EQUAL:
        return ASSIGNMENT_TOKEN_GROUP;
    case BANG:
    case TILDE:
        return UNARY_TOKEN_GROUP;
    case IF:
    case ELSE:
    case FUNC:
    case VAR:
    case PRINT:
        return KEYWORD_TOKEN_GROUP;
    case TRUE:
    case FALSE:
    case STRING:
    case NUMBER:
        return LITERAL_TOKEN_GROUP;
    case OPEN_PAREN:
    case CLOSE_PAREN:
    case OPEN_CURLY:
    case CLOSE_CURLY:
    case SEMICOLON:
    case COMMA:
        return SYNTAX_TOKEN_GROUP;
    default:
        return t;
    }
}

struct Position
{
    unsigned int line;
    unsigned int column;
};

struct Token {
    Token* nextToken;
    TokenType type;
    std::string text;
    Position position;
};

class Lexer {
public:
    explicit Lexer (std::basic_istream<char> &stream);
    Token* getTokenList();
private:
    enum TokenBuildingMode
    {
        BUILDING_NONE,
        BUILDING_NUMBER,
        BUILDING_STRING,
        BUILDING_KEYWORD_OR_IDENTIFIER,
        BUILDING_OPERATOR,
    };
    unsigned int lineOffset = 0;
    unsigned int lineCount = 0;
    std::vector<char> text;
    Token* tokensRoot;
    Token* lastToken;
    void addToken(TokenType type, unsigned int contentOffset, unsigned int contentSize);
    void updateBuildMode(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);
    void processChar(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);
    [[nodiscard]] static TokenType getSyntaxType(char c);
    [[nodiscard]] TokenType getOperatorType(unsigned int start, unsigned int size) const;
    [[nodiscard]] TokenType getKeywordType(unsigned int start, unsigned int size) const;
};



#endif //LEXER_H
