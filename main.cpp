#include <iostream>
#include <fstream>

#include "Lexer.h"


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
    std::cout << lexer.toString();

    return 0;
}
