//
// Created by Ezra Kahn on 5/1/26.
//

#include "Lexer.h"
#include "vector"



bool isWhitespace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

// Future: isOperator and isSyntax can be optimized by ordering most common chars first and perhaps by value ranges
[[nodiscard]] bool isOperator(const char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
        c == '&' || c == '%' || c == '<' || c == '>' || c == '!' || c == '^';
}

[[nodiscard]] bool isSyntax(const char c)
{
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

[[nodiscard]] bool isNumber(const char c)
{
    return 48 <= c && c <= 57;
}

[[nodiscard]] bool isLetter(const char c)
{
    return (65 <= c && c <= 90) || (97 <= c && c <= 122);
}

[[nodiscard]] bool isValidStringContents(const char c)
{
    return 32 <= c && c <= 127;
}

TokenType Lexer::getSyntaxType(char c)
{
    switch (c)
    {
    case '(': return OPEN_PAREN;
    case ')': return CLOSE_PAREN;
    case '{': return OPEN_CURLY;
    case '}': return CLOSE_CURLY;
    case ';': return SEMICOLON;
    case ',': return COMMA;
    default:  return INVALID;
    }
}

[[nodiscard]] TokenType Lexer::getOperatorType(const unsigned int start, const unsigned int size) const
{
    if (size == 1)
    {
        switch (text[start])
        {
        case '!': return BANG;
        case '~': return TILDE;
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return STAR;
        case '/': return FRONT_SLASH;
        case '=': return EQUAL;
        case '&': return AMP;
        case '%': return PERCENT;
        case '<': return LESS_THAN;
        case '>': return GREATER_THAN;
        case '^': return CARET;
        default: return INVALID;
        }
    } if (size == 2)
    {
        switch (text[start])
        {
        case '+':
            if (text[start + 1] == '=') return PLUS_EQUAL;
            break;
        case '-':
            if (text[start + 1] == '=') return MINUS_EQUAL;
            break;
        case '*':
            if (text[start + 1] == '=') return STAR_EQUAL;
            break;
        case '/':
            if (text[start + 1] == '=') return FRONT_SLASH_EQUAL;
            break;
        case '=':
            if (text[start + 1] == '=') return EQUAL_EQUAL;
            break;
        case '&':
            if (text[start + 1] == '&') return AMP_AMP;
            break;
        case '%':
            if (text[start + 1] == '=') return PERCENT_EQUAL;
            break;
        case '<':
            if (text[start + 1] == '=') return LESS_THAN_EQUAL;
            break;
        case '>':
            if (text[start + 1] == '=') return GREATER_THAN_EQUAL;
            break;
        case '!':
            if (text[start + 1] == '=') return BANG_EQUAL;
            break;
        case '^':
            if (text[start + 1] == '=') return CARET_EQUAL;
            break;
        default: return INVALID;
        }
    }
    return INVALID;
}

// future optimization: switch based on size first
TokenType Lexer::getKeywordType(unsigned int start, unsigned int size) const
{
    const std::string_view label(text.data() + start, size);
    if (label == "if") return IF;
    if (label == "else") return ELSE;
    if (label == "func") return FUNC;
    if (label == "var") return VAR;
    if (label == "true") return TRUE;
    if (label == "false") return FALSE;
    return INVALID;
}


void Lexer::addToken(const TokenType type, const unsigned int contentOffset, const unsigned int contentSize)
{
    auto* newToken = new Token{nullptr, type, std::string(&text[contentOffset], contentSize)};
    if (tokensRoot == nullptr)
    {
        tokensRoot = newToken;
        lastToken = tokensRoot;
    } else
    {
        lastToken -> nextToken = newToken;
        lastToken = newToken;
    }
}


//  keeps track of what kind of token is being built from the stream
void Lexer::updateBuildMode(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode)
{
    if (isWhitespace(c))
        buildingMode = BUILDING_NONE;
    else if (isOperator(c))
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
        buildStart = text.size() + 1; // quote not included in token
    } else if (isSyntax(c))
    {
        const unsigned int offset = text.size();
        addToken(getSyntaxType(c), offset, 1);
        buildingMode = BUILDING_NONE;
    } else if (isLetter(c))
    {
        buildingMode = BUILDING_KEYWORD_OR_IDENTIFIER; // keywords or identifiers can't be discerned immediately
        buildStart = text.size();
    } else // Invalid char?
    {
        std::cerr << "Invalid char";
        abort();
    }
}

// Note for future: when token is created but c hasn't been pushed,
// there is temporarily a token referencing char outside of vector,
// if something happens between token creation and vector add c,
// could cause out-of-bounds access
void Lexer::processChar(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode)
{
    switch (buildingMode)
    {
    case BUILDING_NUMBER:
        if (!isNumber(c)) // End of number
        {
            const unsigned int size = text.size() - buildStart;
            addToken(NUMBER, buildStart, size);
            updateBuildMode(c, buildStart, buildingMode);
        }
        break;
    case BUILDING_STRING:
        if (c == '"') // End of string
        {
            const unsigned int size = text.size() - buildStart;
            addToken(STRING, buildStart, size);
            buildingMode = BUILDING_NONE;
        }
        break;
    case BUILDING_OPERATOR:
        if (!isOperator(c)) // End of operator
        {
            unsigned int size = text.size() - buildStart;
            TokenType type = getOperatorType(buildStart, size);
            if (type == INVALID)
            {
                // If token is not valid operator try splitting into to ops: a==!b: "==!" is invalid, but == and ! are valid
                size--;
                type = getOperatorType(buildStart, size);
                const unsigned int secondTokenIndex = text.size() - 1;
                const TokenType secondType = getOperatorType(secondTokenIndex, 1);
                addToken(type, buildStart, size);
                addToken(secondType, secondTokenIndex, 1);

            } else
            {
                addToken(type, buildStart, size);
            }
            updateBuildMode(c, buildStart, buildingMode);
        }
        break;
    case BUILDING_KEYWORD_OR_IDENTIFIER:
        if (!isLetter(c) && !isNumber(c)) // end of keyword/operator
        {
            unsigned int size = text.size() - buildStart;
            TokenType type = getKeywordType(buildStart, size);
            if (type == INVALID) type = IDENTIFIER; // if not keyword, assume identifier
            addToken(type, buildStart, size);
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
    processChar(' ', buildStart, buildingMode);
}

Token* Lexer::getTokenList()
{
    return tokensRoot;
}