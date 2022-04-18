// # Compute the x'th fibonacci number.
// def fib(x)
//   if x < 3 then
//     1
//   else
//     fib(x-1)+fib(x-2)

// # This expression will compute the 40th number.
// fib(40)

#include <string>

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
    return (c == ' ');
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

static int gettok()
{
    static char LastChar = ' ';
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
        IdentifierStr += LastChar;
        if (LastChar == '.')    // start with . 
        {
            while (IsNum(LastChar = getchar()))
                IdentifierStr += LastChar;
        }
        else
        {
            // start with 0
            if (LastChar == '0')
            {
                // must input .
                LastChar = getchar();
                if (LastChar != '.')
                {
                    printf("")
                }
            }
        }
    }
}