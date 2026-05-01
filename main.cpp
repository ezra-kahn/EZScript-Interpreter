#include <iostream>
#include <fstream>


int main(int argc, char* argv[])
{
    std::ifstream file;
    file = std::ifstream("/Users/ezrakahn/CLionProjects/interpreter/test.ez");

    if (!file)
    {
        std::cout << "File load failed!" << std::endl;
        return -1;
    }

    char c;
    while (file.get(c))
    {
        std::cout << c;
    }

    return 0;
}