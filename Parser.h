//
// Created by Ezra Kahn on 5/2/26.
//

#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>

#include "Lexer.h"


enum Operation
{
    UNARY_OP, BINARY_OP, BRANCH_OP, RETRIEVAL_OP, ASSIGNMENT_OP, DECLARATION_OP, LITERAL_OP, INVALID_OP,
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
        std::string_view text;
    } value;
};


struct ASTNode
{
    ASTNode* firstChild;
    ASTNode* nextSibling;
    Operation opClass;
    TokenType opType;
    union Context
    {
        std::string_view valueLabel; // For retrieval_op and assignment_op
        ValueStore value; // For literal_op
        bool tightlyBound; // For binary_op precedence
    } context;
};

class Parser {
public:
    explicit Parser(Token* firstToken);
private:
    Token* currentToken;
    Token* expect(TokenType t, std::string_view componentName);
    ASTNode* expect(ASTNode* n, std::string_view componentName);
    Token* eatToken();
    TokenType glanceToken() const;
    TokenType glanceTokenGroup() const;
    ASTNode* matchExpr();
    ASTNode* matchLiteral();
    std::unordered_map<std::string, ValueStore> valueMap;
};



#endif //PARSER_H
