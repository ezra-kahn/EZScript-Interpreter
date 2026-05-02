//
// Created by Ezra Kahn on 5/1/26.
//

#include "Lexer.h"
#include "Token.h"
#include "vector"



bool isWhitespace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

// Future: isOperator and isSyntax can be optimized by ordering most common chars first and perhaps by value ranges
bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
        c == '&' || c == '%' || c == '<' || c == '>' || c == '!';
}

bool isSyntax(char c)
{
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

bool isNumber(char c)
{
    return (48 <= c) && (c <= 57);
}

//  keeps track of what kind of token is being built from the stream
void Lexer::updateBuildMode(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode)
{
    if (isWhitespace(c))
        buildingMode = BUILDING_NONE;
    else if (isSyntax(c))
    {
        buildingMode = BUILDING_OPERATOR;
        buildStart = text.size(); // Note: text.size is to-be index of cur char
    } else if (isNumber(c))
    {
        buildingMode = BUILDING_NUMBER;
        buildStart = text.size();
    } else if (c == '"')
    {
        buildingMode = BUILDING_STRING;
        buildStart = text.size() + 1; // quote no included in token
    }else
    {
        buildingMode = BUILDING_KEYWORD_OR_IDENTIFIER; // keywords or identifiers can't be discerned immediately
        buildStart = text.size();
    }
}

void Lexer::processChar(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode)
{
    switch (buildingMode)
    {
    case BUILDING_NUMBER:
        if (!isNumber(c))
        {
            const unsigned int size = text.size() - buildStart;
            tokens.push_back(Token{NUMBER, buildStart, size});
            updateBuildMode(c, buildStart, buildingMode);
        }
        break;
    case BUILDING_NONE:
    default:
        updateBuildMode(c, buildStart, buildingMode);
    }
    text.push_back(c);
}

Lexer::Lexer(std::basic_istream<char> &stream)
{
    char c;
    TokenBuildingMode buildingMode = BUILDING_NONE;
    unsigned int buildStart = 0;
    while (stream.get(c))
    {
        processChar(c, buildStart, buildingMode);
    }
    processChar('\0', buildStart, buildingMode);
}

std::string Lexer::toString() const
{
    std::string out;
    for (Token token: tokens)
    {
        out.append("Number");
    }
    return out;
}
