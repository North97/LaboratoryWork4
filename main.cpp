#include "syntaxanalyzer.h"

int main()
{
    SyntaxAnalyzer analyzer;
    std::ifstream in("program.lng", std::ios::in);
    analyzer.run(in);
    in.close();
    return 0;
}
