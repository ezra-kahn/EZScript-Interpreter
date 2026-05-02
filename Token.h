//
// Created by Ezra Kahn on 5/1/26.
//

#ifndef TOKEN_H
#define TOKEN_H

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

    // Single char ops: + - * / = & % < >
    PLUS, MINUS, STAR, FRONT_SLASH, EQUAL, AMP, PERCENT, LESS_THAN, GREATER_THAN,

    // Two char ops: += -= *= /= == && <= >=
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, FRONT_SLASH_EQUAL, EQUAL_EQUAL, AMP_AMP, LESS_EQUAL, GREATER_EQUAL,

    INVALID,
};


struct Token {
    TokenType type;
    unsigned int contentOffset;
    unsigned int contentSize;
};
#endif //TOKEN_H
