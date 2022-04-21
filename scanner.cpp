// # Compute the x'th fibonacci number.
// def fib(x)
//   if x < 3 then
//     1
//   else
//     fib(x-1)+fib(x-2)

// # This expression will compute the 40th number.
// fib(40)

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum Token {
    tok_eof =   -1,

    // commands
    tok_def =   -2,
    tok_extern  =   -3,

    // primary
    tok_identifier  =   -4,
    tok_number  =   -5,
};

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;

bool IsSpace(char c)
{
    return (c == ' ' || c == '\t');
}

// Recognize an Alpha Character
bool IsAlpha(char c)
{
    // [A .. Z]: 65 ~ 90
    // [a .. z]: 97 ~ 122
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

// Recognize a Decimal Digit
bool IsNum(char c)
{
    // [0 .. 9]: 48 ~ 57
    return (c >= 48 && c <= 57);
}

bool IsAlNum(char c)
{
    return IsAlpha(c) || IsNum(c);
}

void Abort(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);

    exit(1);
}

static int gettok()
{
    static int LastChar = ' ';
    IdentifierStr.clear();

    while (IsSpace(LastChar))
        LastChar = getchar();

    if (IsAlpha(LastChar) || LastChar == '_')  // Identifier: [a-zA-Z_][a-zA-Z0-9_]*
    {
        IdentifierStr += LastChar;
        while (IsAlNum(LastChar = getchar()) || LastChar == '_')
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;

        if (IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }
    else if (IsNum(LastChar) || LastChar == '.')    // Number: [0-9]*[.]?[0-9]*
    {
        std::string str_val;
        str_val += LastChar;

        while (IsNum(LastChar = getchar()) || LastChar == '.')
                str_val += LastChar;
        NumVal = std::stod(str_val);
        return tok_number;
    }
    else if (LastChar == '#')
    {
        // Comment
        while ((LastChar = getchar()) != '\n' && LastChar != '\r');
        return gettok();
    }
    else if (LastChar == '\n' || LastChar == '\r')
    {
        while ((LastChar = getchar()) == '\n' || LastChar == '\r');
        return gettok();
    }
    else if (LastChar != EOF)
    {
        IdentifierStr += LastChar;
        LastChar = getchar();
        return tok_identifier;
    }
    else
    {
        return tok_eof;
    }
}

int main()
{
    int tok;
    while (tok_eof != (tok = gettok()))
    {
        if (tok == tok_def)
            printf("tok_def: %s\n", IdentifierStr.c_str());
        else if (tok == tok_extern)
            printf("tok_extern: %s\n", IdentifierStr.c_str());
        else if (tok == tok_identifier)
            printf("tok_identifier: %s\n", IdentifierStr.c_str());
        else if (tok == tok_number)
            printf("tok_number: %f\n", NumVal);
    }
    printf("tok_eof\n");

    return 0;
}