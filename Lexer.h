//
// Created by Ezra Kahn on 5/1/26.
//

#ifndef LEXER_H
#define LEXER_H

#include "iostream"
#include "Token.h"
#include "vector"

class Lexer {
public:
    explicit Lexer (std::basic_istream<char> &stream);
    [[nodiscard]] std::string toString() const;
private:
    enum TokenBuildingMode
    {
        BUILDING_NONE,
        BUILDING_NUMBER,
        BUILDING_STRING,
        BUILDING_KEYWORD_OR_IDENTIFIER,
        BUILDING_OPERATOR,
    };

    std::vector<char> text;
    std::vector<Token> tokens;
    void updateBuildMode(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);
    void processChar(char c, unsigned int &buildStart, TokenBuildingMode &buildingMode);

};



#endif //LEXER_H
