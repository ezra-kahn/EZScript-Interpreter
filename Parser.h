//
// Created by Ezra Kahn on 5/2/26.
//

#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>

#include "Lexer.h"


enum Operation
{
    STATEMENT_GROUP, PRINT_OP, UNARY_OP, BINARY_OP, BRANCH_OP, RETRIEVAL_OP, ASSIGNMENT_OP, DECLARATION_OP, LITERAL_OP, INVALID_OP, LOOP_OP
};

enum ValueType
{
    NO_VALUE, INTEGER_VALUE, BOOLEAN_VALUE, TEXT_VALUE
};

struct ValueStore
{
    ValueType valueType;
    union Value
    {
        int integer;
        bool boolean;
        std::string_view text;
    } value;


};

constexpr int precedence(const TokenType t)
{
    switch (t)
    {
    case OPEN_PAREN:
        return 7;
    case STAR:
    case FRONT_SLASH:
    case PERCENT:
        return 6;
    case PLUS:
    case MINUS:
        return 5;
    case LESS_THAN:
    case LESS_THAN_EQUAL:
    case GREATER_THAN:
    case GREATER_THAN_EQUAL:
        return 4;
    case EQUAL_EQUAL:
    case BANG_EQUAL:
        return 3;
    case AMP:
        return 2;
    case CARET:
        return 1;
    default:
        return -1;
    }
}

struct ASTNode
{
    ASTNode* firstChild;
    ASTNode* nextSibling;
    Operation opClass;
    TokenType opType;
    union Context
    {
        std::string_view valueLabel; // For retrieval_op and assignment_op
        ValueStore valueStore; // For literal_op
    } context;
};

class Parser {
public:
    explicit Parser(Token* firstToken);
    ASTNode* parseExprClosed();
    static void printTree(ASTNode* node);

    ASTNode* getAST()
    {
        return expect(parseStatementGroup(), "STATEMENT");
    }

private:
    ASTNode* parseExpr(int minPrec = 0); // Pratt Parser
    ASTNode* parseLiteral();
    ASTNode* parseIdentifier();
    ASTNode* parsePrefix();
    ASTNode* parseStatement();
    ASTNode* parseDeclaration();
    ASTNode* parseStatementGroup();
    ASTNode* parseAssignment();
    ASTNode* parsePrint();
    ASTNode* parseBlock();
    ASTNode* parseBranch();
    ASTNode* parseLoop();
    ASTNode* parseWhile();

    Token* currentToken;
    Token* expect(TokenType t, std::string_view componentName = "");
    ASTNode* expect(ASTNode* n, std::string_view componentName = "");
    Token* eatToken();
    TokenType glanceToken() const;
    TokenType glanceTokenGroup() const;
    static ASTNode* binary(ASTNode* left, TokenType op, ASTNode* right);
};



#endif //PARSER_H
