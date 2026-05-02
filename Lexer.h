//
// Created by Ezra Kahn on 5/1/26.
//

#ifndef LEXER_H
#define LEXER_H

#include "iostream"
#include "vector"

enum TokenType
{
    // Literals: "abc" 123
    STRING, NUMBER,

    // Identifiers: varName
    IDENTIFIER,

    // Keywords: if else function var true false
    IF, ELSE, FUNCTION, VAR, TRUE, FALSE,

    // Syntax Chars: ( ) { } ; ,
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURLY, CLOSE_CURLY, SEMICOLON, COMMA,

    // Unary ops (must be single char): ! ~
    BANG, TILDE,

    // Single char ops: + - * / = & % < > ^
    PLUS, MINUS, STAR, FRONT_SLASH, EQUAL, AMP, PERCENT, LESS_THAN, GREATER_THAN, CARET,

    // Two char ops: += -= *= /= == && %= <= >= != ^=
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, FRONT_SLASH_EQUAL, EQUAL_EQUAL, AMP_AMP,
    PERCENT_EQUAL, LESS_THAN_EQUAL, GREATER_THAN_EQUAL, BANG_EQUAL, CARET_EQUAL,

    INVALID,
};

struct Token {
    TokenType type;
    unsigned int contentOffset;
    unsigned int contentSize;
};

class Lexer {
public:
    explicit Lexer (std::basic_istream<char> &stream);
    [[nodiscard]] std::string toString() const;
private:
    enum TokenBuildingMode
    {
        BUILDING_NONE,
        BUILDING_NUMBER,
        BUILDING_STRING,
        BUILDING_KEYWORD_OR_IDENTIFIER,
        BUILDING_OPERATOR,
    };

    std::vector<char> text;
    std::vector<Token> tokens;
    void updateBuildMode(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);
    void processChar(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);
    [[nodiscard]] static TokenType getSyntaxType(char c);
    [[nodiscard]] TokenType getOperatorType(unsigned int start, unsigned int size) const;
};



#endif //LEXER_H
