#include <string>
#include <memory>

class ExprAST
{
public:
    virtual ~ExprAST() {}
};

// 数值
class NumberExprAST : public ExprAST
{
public:
    NumberExprAST(double Val): Val(Val) {}

    double Val;
};

// 变量
class VariableExprAST : public ExprAST
{
public:
    VariableExprAST(const std::string& Name): Name(Name) {}

    std::string Name;
};

// 二目运算符
class BinaryExprAST : public ExprAST
{
public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> )
};