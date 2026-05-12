//
// Created by Ezra Kahn on 5/2/26.
//

#include "Parser.h"
#include "inc/magic_enum/magic_enum_all.hpp"

// Recursive Descent Parse Methods

ASTNode* Parser::parseDeclaration()
{
    if (glanceToken() != VAR) return nullptr;
    eatToken();
    ASTNode* declRoot = expect(parseIdentifier(), "NAME");
    declRoot -> opClass = DECLARATION_OP;
    expect(EQUAL);
    ASTNode* val = expect(parseExpr());
    declRoot -> firstChild = val;
    expect(SEMICOLON);
    return declRoot;
}


ASTNode* Parser::parseStatement()
{
    ASTNode* statement;
    if (currentToken == nullptr) return nullptr;
    if ((statement = parseDeclaration())) return statement;
    if ((statement = parsePrint())) return statement;
    //return parseDeclaration() || parseAssignment() || parseBranch();
    return nullptr;
}

ASTNode* Parser::parseStatementGroup()
{
    ASTNode* firstStatement = parseStatement();
    if (firstStatement == nullptr) return nullptr;
    ASTNode* statement = firstStatement;
    while ((statement->nextSibling = parseStatement()))
    {
        statement = statement->nextSibling;
    }
    return new ASTNode{firstStatement, nullptr, STATEMENT_GROUP, NO_TOKEN_GROUP};
}

ASTNode* Parser::parsePrint()
{
    if (glanceToken() != PRINT) return nullptr;
    eatToken();
    ASTNode* printVal = expect(parseExprClosed());
    expect(SEMICOLON);
    return new ASTNode{printVal, nullptr, PRINT_OP, NO_TOKEN_GROUP};
}

ASTNode* Parser::parseExprClosed()
{
    if (currentToken->type != OPEN_PAREN)
        return nullptr;
    eatToken();
    ASTNode* expr = parseExpr();
    expect(CLOSE_PAREN);
    return expr;
}

ASTNode* Parser::parseExpr(int minPrec)
{
    ASTNode* left = parsePrefix();
    if (left == nullptr)
        return nullptr;
    while (true)
    {
        int prec = precedence(glanceToken());

        if (prec <= minPrec)
            break;

        Token* opToken = expect(BINARY_TOKEN_GROUP);

        ASTNode* right = expect(parseExpr(prec), "EXPRESSION");
        left = binary(left, opToken->type, right);
    }
    return left;
}

ASTNode* Parser::parsePrefix()
{
    // Identifier, Literal, Unary Op
    TokenType tokenGroup = glanceTokenGroup();
    if (tokenGroup == LITERAL_TOKEN_GROUP)
    {
        return parseLiteral();
    }
    if (tokenGroup == IDENTIFIER)
    {
        return parseIdentifier();
    }
    if (tokenGroup == UNARY_TOKEN_GROUP)
    {
        TokenType op = eatToken()->type;
        ASTNode* child = parseExpr();
        if (child == nullptr) return nullptr;
        return new ASTNode{child, nullptr, UNARY_OP, op, };
    }
    if (glanceToken() == OPEN_PAREN)
    {
        eatToken();
        ASTNode* prefix = parseExpr();
        expect(CLOSE_PAREN, "CLOSING PREFIX");
        return prefix;
    }
    return nullptr;
}

ASTNode* Parser::parseLiteral()
{
    if (glanceTokenGroup() != LITERAL_TOKEN_GROUP) return nullptr;
    const Token* t = expect(NUMBER, "LITERAL");
    auto* node = new ASTNode{nullptr, nullptr, LITERAL_OP, t->type,};
    node->context.valueStore = {INTEGER_VALUE, std::stoi(t->text)};
    return node;
}

ASTNode* Parser::parseIdentifier()
{
    if (glanceToken() != IDENTIFIER) return nullptr;
    const Token* t = eatToken();
    auto* node = new ASTNode{nullptr, nullptr, RETRIEVAL_OP, NO_TOKEN_GROUP,};
    node->context.valueLabel = t->text;
    return node;
}

// HELPER METHODS



ASTNode* Parser::binary(ASTNode* left, TokenType op, ASTNode* right)
{
    left->nextSibling = right;
    return new ASTNode{left, nullptr, BINARY_OP, op};
}

Parser::Parser(Token* firstToken)
{
    currentToken = firstToken;
    ASTNode* expr = expect(parseStatementGroup(), "EXPRESSION");
    printTree(expr);
}

void Parser::printTree(ASTNode* node)
{
    std::cout << "(" << magic_enum::enum_name(node -> opClass);
    if (node -> opClass == BINARY_OP || node -> opClass == UNARY_OP)
    {
        std::cout << ": " << magic_enum::enum_name(node -> opType);
    } else if (node -> opClass == LITERAL_OP)
    {
        std::cout << ": ";
        printVal(node->context.valueStore);
    } else if (node -> opClass == RETRIEVAL_OP)
    {
        std::cout << ": '" << node->context.valueLabel << "'";
    } else if (node -> opClass == DECLARATION_OP)
    {
        std::cout << ": '" << node->context.valueLabel << "'";
    }
    std::cout << ": ";
    auto* it = node -> firstChild;
    while (it != nullptr)
    {
        printTree(it);
        std::cout << ", ";
        it = it->nextSibling;
    }
    std::cout << ")";
}

void Parser::printVal(ValueStore v)
{
    switch (v.valueType)
    {
    case TEXT_VALUE:
        std::cout << v.value.text;
    case FLOATING_VALUE:
        std::cout << v.value.floating;
    case INTEGER_VALUE:
    default:
        std::cout << v.value.integer;
    }
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

void printPositionErr(Token* t)
{
    std::cerr << "Position - line: " << t->position.line + 1 << " column: " << t->position.column + 1 << std::endl;
}

Token* Parser::expect(TokenType t, std::string_view componentName)
{
    if (currentToken == nullptr)
    {
        std::cerr << "SYNTAX ERROR - " << componentName << " - EXPECTED: " << magic_enum::enum_name(t) << " BUT GOT EOF" << std::endl;
        printPositionErr(currentToken);
        abort();
    }
    if (t == currentToken->type || t == getTokenGroup(currentToken->type))
    {
        return eatToken();
    } else
    {
        std::cerr << "SYNTAX ERROR - " << componentName << " - EXPECTED: " << magic_enum::enum_name(t)
        << " - GOT: " << magic_enum::enum_name(currentToken->type) << " OF CLASS " << magic_enum::enum_name(getTokenGroup(currentToken->type)) << std::endl;
        printPositionErr(currentToken);
        abort();
    }
}

ASTNode* Parser::expect(ASTNode* n, std::string_view componentName)
{
    if (n == nullptr)
    {
        std::cerr << "SYNTAX ERROR - EXPECTED: " << componentName << std::endl;
        printPositionErr(currentToken);
        abort();
    } else
    {
        return n;
    }
}

