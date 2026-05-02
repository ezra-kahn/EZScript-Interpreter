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

    // Single char ops: + - * / =
    PLUS, MINUS, STAR, FRONT_SLASH, EQUAL,

    // Syntax Chars: ( ) { } ; ,
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURLY, CLOSE_CURLY, SEMICOLON, COMMA,

    // Two char ops: += -= *= /= ==
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, FRONT_SLASH_EQUAL, EQUAL_EQUAL,

    INVALID,
};


struct Token {
    TokenType type;
    int contentOffset;
    int contentSize;
};
#endif //TOKEN_H
