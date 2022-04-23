#include "scanner.h"
#include "lexer.hpp"
#include <stdio.h>

static void MainLoop()
{
    fprintf(stderr, "Kal > ");
    getNextToken();
    while (1)
    {
        fprintf(stderr, "Kal > ");
        switch (CurTok)
        {
        case tok_eof:
            return;
        case ';':
            getNextToken();
            break;
        case tok_extern:
            HandleExtern();
            break;
        case tok_def:
            HandleDefinition();
            break;
        default:
            HandleExpression();
            break;
        }
    }
}

int main(int argc, char** argv)
{
    MainLoop();

    return 0;
}