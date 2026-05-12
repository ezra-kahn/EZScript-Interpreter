#include <iostream>
#include <fstream>

#include "Evaluator.h"
#include "Lexer.h"
#include "Parser.h"


int main(int argc, char* argv[])
{
    std::ifstream file;
    file = std::ifstream("/Users/ezrakahn/CLionProjects/interpreter/test.ez");

    if (!file)
    {
        std::cout << "File load failed!" << std::endl;
        return -1;
    }

    Lexer lexer(file);
    Parser parser(lexer.getTokenList());
    Evaluator evaluator(parser.getAST());
    evaluator.evaluateCode();

    return 0;
}
