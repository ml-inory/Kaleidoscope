#include "lexer.hpp"
#include <map>

static int CurTok;

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

static int getNextToken()
{
    return (CurTok = gettok());
}

static std::unique_ptr<ExprAST> LogError(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    fprintf(stderr, "LogError: %s\n", fmt, argptr);
    va_end(argptr);

    return nullptr;
}

static std::unique_ptr<PrototypeExprAST> LogErrorP(const char* fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    LogError(fmt, argptr);
    va_end(argptr);

    return nullptr;
}

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberAST()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

// +-*/
static bool IsBinaryOp()
{
    return (IdentifierStr == "+" || IdentifierStr == "-" || IdentifierStr == "*" || IdentifierStr == "/");
}

// binaryexpr ::= var/num op val/num
static std::unique_ptr<ExprAST> ParseBinaryAST()
{
    char Op = IdentifierStr.data()[0];
    // auto LHS = std::move(LastExpr);
    // getNextToken();
    // auto RHS = ParseExpression();
}

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

            Args.push_back(expr);

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

// prototype expr ::= extern name(expressions)
static std::unique_ptr<ExprAST> ParsePrototypeAST()
{
    getNextToken();

    if (CurTok != tok_identifier)
        return LogErrorP("expected identifier");

    std::string Name = IdentifierStr;

    getNextToken();
    if (CurTok != '(')
        return LogErrorP("expected '('");

    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (1)
        {
            auto arg = ParseExpression();
            if (arg == nullptr)
                return LogErrorP("argument %d for %s is NULL", Args.size() + 1, Name.c_str());

            Args.push_back(arg);

            getNextToken();
            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogErrorP("expected ','");
        }
    }

    getNextToken();

    return std::make_unique<PrototypeExprAST>(Name, std::move(Args));
}

// function impl ::= def Name(expressions)
static std::unique_ptr<ExprAST> ParseFunctionAST()
{
    getNextToken();

    if (CurTok != tok_identifier)
        return LogError("expected identifier");

    std::string Name = IdentifierStr;

    getNextToken();
    if (CurTok != '(')
        return LogError("expected '('");

    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (1)
        {
            auto arg = ParseExpression();
            if (arg == nullptr)
                return LogError("argument %d for %s is NULL", Args.size() + 1, Name.c_str());

            Args.push_back(arg);

            getNextToken();
            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("expected ','");
        }
    }

    getNextToken();

    return std::make_unique<FunctionAST>(Name, std::move(Args));
}

// binop ::= + num/var
static std::unique_ptr<ExprAST> ParseBinop()
{
    
}

static std::unique_ptr<ExprAST> ParsePrimary()
{
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
    case tok_extern:
    {
        return ParsePrototypeAST();
    }
    case tok_def:
    {
        return ParseFunctionAST();
    }
        
    }

    if (BinopPrecedence.find(CurTok) != BinopPrecedence.end())
        return ParseBinop();

    return nullptr;
}

// expression
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto Result = ParsePrimary();
    getNextToken();
    return Result;
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

    return V;
}