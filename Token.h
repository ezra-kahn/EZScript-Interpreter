//
// Created by Ezra Kahn on 5/1/26.
//

#ifndef TOKEN_H
#define TOKEN_H

enum TokenType
{
    // Literals:
    STRING, NUMBER, IDENTIFIER,

    // Keywords:
    IF, ELSE, FUNCTION, VAR,

    // Binary Operators:
    ADD, SUBTRACT, MULTIPLY, DIVIDE,

    // Syntax Chars:
    OPEN_PAREN, CLOSE_PAREN, OPEN_CURLY, CLOSE_CURLY, SEMICOLON
};

struct Token { // EX: STRING, 5, HELLO
    TokenType type;
    int content_size;
    char* contents;
};

#endif //TOKEN_H
