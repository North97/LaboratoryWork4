#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{

}

int SyntaxAnalyzer::toSymbolClass(char &value)
{
    if(value == '\n' || value == ' ' || value == '\t')
    {
        return 0;
    }
    else if(isdigit(value))
    {
        return 1;
    }
    else if(isalpha(value))
    {
        return 2;
    }
    else if(value == '=')
    {
        return 3;
    }
    else if(value == ';')
    {
        return 4;
    }
    else if(value == '+' || value == '-')
    {
        return 5;
    }
    else if(value == '*' || value == '\\')
    {
        return 6;
    }
    else
    {
        return 7;
    }
}

void SyntaxAnalyzer::run(std::ifstream &in)
{
    std::string temp;
    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string code = buffer.str();
    buffer.str("");


    std::cout << "Source code:\n" << code << "---------------------------------------------------------\n";

    int beforeState;
    int currentState = lexicalStateMachine[toSymbolClass(code[0])][0];

    for(int i = 0; i < (int)code.length(); ++i)
    {
        beforeState = currentState;
        currentState = lexicalStateMachine[toSymbolClass(code[i])][currentState];

        if(separators.find(code[i]) != separators.end())
        {
            temp = buffer.str();
            buffer.str("");

            if(temp != std::string())
            {
                resultLexeme.push_back(temp);
                codetLexeme.push_back(toLexemeClas(beforeState, temp));
            }

            temp = code[i];

            if(insignSeparators.find(code[i]) == insignSeparators.end())
            {
                resultLexeme.push_back(temp);
                codetLexeme.push_back(toLexemeClas(currentState, temp));
            }
        }
        else
        {
            buffer << code[i];
        }
    }

    currentState = 0;
    beforeState = 0;
    for (int i = 0; i < (int)codetLexeme.size(); ++i)
    {
        beforeState = currentState;
        currentState = syntaxStateMachine[codetLexeme[i]][currentState];
        if(currentState == -1)
        {
            break;
        }

        std::cout << resultLexeme[i] << " ";
    }
    if(currentState == 12)
    {
        std::cout << "\n\nAll right." << currentState;
    }
    else if(currentState != -1)
    {
        std::cout << "\n\nUnexpected lexeme. Expected:\n" << expectedLexeme(currentState) << "\n\n";
    }
    else
    {
        std::cout << "\n\nUnexpected lexeme. Expected:\n" << expectedLexeme(beforeState) << "\n\n";
    }
}

std::string SyntaxAnalyzer::expectedLexeme(int &value)
{
    switch (value) {
    case 0:
        return "Keyword select";
    case 1:
        return "Identifire or constant";
    case 2:
        return "Keyword case or any arithmetic operation";
    case 3:
        return "Constant";
    case 4:
        return "Identifire";
    case 5:
        return "Assignment operator";
    case 6:
        return "Identifire or constant";
    case 7:
        return "Keyword case/default/end, separator ; or any arithmetic operation";
    case 8:
        return "Identifire";
    case 9:
        return "Assignment operator";
    case 10:
        return "Identifire or constant";
    case 11:
        return "Keyword end, separator ; or any arithmetic operation";
    }
}

int SyntaxAnalyzer::toLexemeClas(int &value, std::string &lex)
{
    switch (value) {
    case 1:
        if(keywords.find(lex) != keywords.end())
        {
            if(lex == "select")
            {
                return 0;
            }
            else if(lex == "case")
            {
                return 1;
            }
            else if(lex == "default")
            {
                return 2;
            }
            else if(lex == "end")
            {
                return 3;
            }
            else
            {
                return 9;
            }
        }
        else
            return 4;
    case 2:
        return 5;
    case 3:
        return 7;
    case 4:
        return 8;
    case 5:
        return 6;
    case 6:
        return 6;
    default:
        return 9;
    }
}
