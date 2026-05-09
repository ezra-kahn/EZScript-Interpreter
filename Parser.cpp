//
// Created by Ezra Kahn on 5/2/26.
//

#include "Parser.h"
#include "inc/magic_enum/magic_enum_all.hpp"

ASTNode* Parser::matchExpr()
{
    TokenType tokenGroup = glanceTokenGroup();
    if (tokenGroup == UNARY_TOKEN_GROUP)
    {
        Token* op = eatToken();
        ASTNode* childTree = matchExpr();
        auto* newNode = new ASTNode{childTree, nullptr, UNARY_OP, op->type, nullptr};
        return newNode;
    }
    if (tokenGroup == LITERAL_TOKEN_GROUP || tokenGroup == IDENTIFIER_TOKEN_GROUP)
    {
        Token* aToken = eatToken();
        ASTNode* a;
        if (tokenGroup == LITERAL_TOKEN_GROUP)
        {
            a = new ASTNode{nullptr, nullptr, LITERAL_OP, aToken->type,};
        }else // Identifier
        {
            a = new ASTNode{nullptr, nullptr, RETRIEVAL_OP, NO_TOKEN_GROUP,};
            a->context.valueLabel = aToken->text;
        }

        if (glanceTokenGroup() == BINARY_TOKEN_GROUP)
        {
            // LITERAL/IDENT BINARY_TOKEN EXPR
            Token* opToken = eatToken();
            ASTNode* b = expect(matchExpr(), "EXPRESSION");

            // PRECEDENCE SWAPPING

            a->nextSibling = b;

            ASTNode* op = new ASTNode{a, nullptr, BINARY_OP, opToken->type};
            return op;
        } else
        {
            return a;
        }

    }
    if (glanceToken() == OPEN_CURLY){
        eatToken();
        ASTNode* expr = expect(matchExpr(), "EXPRESSION");
        expect(CLOSE_PAREN, "EXPRESSION");
        expr->context.tightlyBound = true;
    }
}

ASTNode* Parser::matchLiteral()
{
}

Parser::Parser(Token* firstToken)
{
    currentToken = firstToken;
    ASTNode* expr = matchExpr();
    std::cout << magic_enum::enum_name(expr->opType);
}

Token* Parser::eatToken()
{
    Token* t = currentToken;
    currentToken = t -> nextToken;
    return t;
}


TokenType Parser::glanceToken() const
{
    if (currentToken == nullptr) return NO_TOKEN_GROUP;
    return currentToken->type;
}

TokenType Parser::glanceTokenGroup() const
{
    return getTokenGroup(glanceToken());
}


Token* Parser::expect(TokenType t, std::string_view componentName)
{
    if (currentToken == nullptr)
    {
        std::cerr << "SYNTAX ERROR - " << componentName << " - EXPECTED: " << magic_enum::enum_name(t) << " BUT GOT EOF";
        abort();
    }
    if (t == currentToken->type || t == getTokenGroup(currentToken->type))
    {
        currentToken = currentToken->nextToken;
        return currentToken;
    } else
    {
        std::cerr << "SYNTAX ERROR - " << componentName << " - EXPECTED: " << magic_enum::enum_name(t)
        << " - GOT: " << currentToken->type << " OF CLASS " << magic_enum::enum_name(getTokenGroup(currentToken->type));
        abort();
    }
}

ASTNode* Parser::expect(ASTNode* n, std::string_view componentName)
{
    if (n == nullptr)
    {
        std::cerr << "SYNTAX ERROR - EXPECTED: " << componentName;
        abort();
    } else
    {
        return n;
    }
}