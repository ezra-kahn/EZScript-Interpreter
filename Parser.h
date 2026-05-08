//
// Created by Ezra Kahn on 5/2/26.
//

#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>

#include "Lexer.h"


enum Operation
{
    UNARY_OP, BINARY_OP, BRANCH_OP, RETRIEVAL_OP, ASSIGNMENT_OP, DECLARATION_OP
};

enum ValueType
{
    INTEGER, DECIMAL, TEXT,
};

struct ValueStore
{
    ValueType valueType;
    union Value
    {
        int integer;
        double decimal;
        std::string text;
    };
};

struct ASTNode
{
    ASTNode* firstChild;
    ASTNode* nextChild;
    Operation opClass;
    union Context
    {
        TokenType opType;
        ValueStore* valueStore; // For retrieval_op and assignment_op
    };
};

class Parser {
public:
    explicit Parser(std::vector<Token> &tokens);
private:
    std::unordered_map<std::string, ValueStore> valueMap;
};


#endif //PARSER_H
