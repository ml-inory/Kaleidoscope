#include <string>
#include <memory>
#include <vector>

class ExprAST
{
public:
    virtual ~ExprAST() {}
};

// 数值
class NumberExprAST : public ExprAST
{
    double Val;

public:
    NumberExprAST(double Val): Val(Val) {}
};

// 变量
class VariableExprAST : public ExprAST
{
    std::string Name;

public:
    VariableExprAST(const std::string& Name): Name(Name) {}
};

// 二目运算符
class BinaryExprAST : public ExprAST
{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS))
    {}
};

// 函数调用
class CallExprAST : public ExprAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string& Callee, const std::vector<std::unique_ptr<ExprAST>>& Args):
    Callee(Callee),
    Args(std::move(Args))
    {}
};

// 函数原型
class PrototypeExprAST : public ExprAST
{
    std::string Name;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    PrototypeExprAST(const std::string& Name, const std::vector<std::unique_ptr<ExprAST>>& Args):
    Name(Name),
    Args(std::move(Args))
    {}
};

// 函数实现
class FunctionAST : public ExprAST
{
    std::unique_ptr<PrototypeExprAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeExprAST> Proto, std::unique_ptr<ExprAST> Body):
    Proto(std::move(Proto)),
    Body(std::move(Body))
    {}
};