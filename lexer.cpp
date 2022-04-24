#include "ast.hpp"
#include "scanner.h"
#include "lexer.hpp"
#include <map>

int CurTok;

// 运算符优先级
static std::map<char, int>  BinopPrecedence {
    {'<', 10},
    {'=', 10},
    {'>', 10},
    {'+', 20},
    {'-', 20},
    {'*', 30},
    {'/', 30}
};

int getNextToken()
{
    CurTok = gettok();
    printf("CurTok: %d\n", CurTok);
    return CurTok;
}

static std::unique_ptr<ExprAST> LogError(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    fprintf(stderr, "LogError: ");
    vfprintf(stderr, fmt, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);

    exit(1);

    return nullptr;
}

static std::unique_ptr<PrototypeExprAST> LogErrorP(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    fprintf(stderr, "LogErrorP: ");
    vfprintf(stderr, fmt, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);

    exit(1);

    return nullptr;
}

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberAST()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static std::unique_ptr<ExprAST> ParseExpression();

// identifierexpr ::= identifier
static std::unique_ptr<ExprAST> ParseIdentifierAST()
{
    std::string Name = IdentifierStr;

    getNextToken();

    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(Name);

    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (1)
        {
            auto expr = ParseExpression();
            if (expr == nullptr)
                return LogError("argument %d for %s is NULL", Args.size() + 1, Name.c_str());

            Args.push_back(std::move(expr));

            getNextToken();
            if (CurTok == ')')
                break;
            
            if (CurTok != ',')
                return LogError("expected ','");
        }
    }

    getNextToken();

    return std::make_unique<CallExprAST>(Name, std::move(Args));
}

// prototype expr ::= extern FnName(ArgNames)
static std::unique_ptr<PrototypeExprAST> ParsePrototypeAST()
{
    if (CurTok != tok_identifier)
        return LogErrorP("expected identifier");

    std::string Name = IdentifierStr;

    getNextToken();
    if (CurTok != '(')
        return LogErrorP("expected '('");

    getNextToken();
    std::vector<std::string> Args;
    if (CurTok != ')')
    {
        while (getNextToken() == tok_identifier)
        {
            Args.push_back(IdentifierStr);
        }
    }

    getNextToken();

    return std::make_unique<PrototypeExprAST>(Name, std::move(Args));
}

static std::unique_ptr<PrototypeExprAST> ParseExtern()
{
    getNextToken();
    return ParsePrototypeAST();
}

// function impl ::= def prototype expressions
static std::unique_ptr<ExprAST> ParseDefAST()
{
    getNextToken();

    std::unique_ptr<PrototypeExprAST> Prototype = ParsePrototypeAST();
    if (!Prototype)
        return LogError("expected prototype");

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Prototype), std::move(E));
    return nullptr;
}

static int GetOpPrecedence()
{
    if (!isascii(CurTok))
        return -1;

    if (BinopPrecedence.find(CurTok) != BinopPrecedence.end())
        return BinopPrecedence[CurTok];

    return -1;
}

// parenexpr ::= ( expression )
static std::unique_ptr<ExprAST> ParseParenAST()
{
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken();

    return std::move(V);
}

static std::unique_ptr<ExprAST> ParsePrimary()
{
    // printf("CurTok: %d\n", CurTok);
    switch (CurTok)
    {
    case tok_number:
    {
        return ParseNumberAST();
    }
    case tok_identifier:
    {
        return ParseIdentifierAST();
    }
    case '(':
    {
        return ParseParenAST();
    }
    default:
        return LogError("expected primary expression");
        
    }
}

// binop ::= + num/var
static std::unique_ptr<ExprAST> ParseBinopRHS(int OpPrec, std::unique_ptr<ExprAST> LHS)
{
    printf("begin\n");
    while (1)
    {
        printf("token: %d\n", CurTok);

        int CurOpPrec = GetOpPrecedence();
        printf("CurOpPrec: %d    OpPrec: %d\n", CurOpPrec, OpPrec);
        if (CurOpPrec < OpPrec)
            return std::move(LHS);

        char Op = CurTok;
        printf("op: %d %c\n", Op, Op);
        if (BinopPrecedence.find(Op) == BinopPrecedence.end())
            return LogError("Unsupported binary op: %c", Op);

        getNextToken();
        auto RHS = ParsePrimary();
        if (!RHS)
            return LogError("expected rhs for binop");

        int NextOpPrec = GetOpPrecedence();
        printf("CurOpPrec: %d    NextOpPrec: %d\n", CurOpPrec, NextOpPrec);
        if (NextOpPrec > CurOpPrec)
        {
            RHS = ParseBinopRHS(CurOpPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }
            
        LHS = std::make_unique<BinaryExprAST>(Op, std::move(LHS), std::move(RHS));
    }
    printf("end\n");
}

// expression
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if (!LHS)
        return LogError("parse primary failed!");

    printf("ParseBinopRHS\n");
    return ParseBinopRHS(0, std::move(LHS));
}

// 零元函数包住整个代码
static std::unique_ptr<FunctionAST> ParseTopLevelAST()
{
    printf("ParseTopLevelAST\n");
    auto E = ParseExpression();
    if (E)
    {
        printf("ParseExpression\n");
        auto Proto = std::make_unique<PrototypeExprAST>("", std::vector<std::string>{});
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    printf("null\n");
    return nullptr;
}

void HandleExtern()
{
    printf("Parsing extern\n");
    if (!ParseExtern())
        fprintf(stderr, "Parse extern error\n");
    else
        fprintf(stderr, "Parsed an extern\n");
}

void HandleDefinition()
{
    printf("Parsing definition\n");
    if (!ParseDefAST())
        fprintf(stderr, "Parse definition error\n");
    else
        fprintf(stderr, "Parsed an definition\n");
}

void HandleExpression()
{
    printf("Parsing expression\n");
    if (!ParseTopLevelAST())
        fprintf(stderr, "Parse expression error\n");
    else
        fprintf(stderr, "Parsed an expression\n");
}