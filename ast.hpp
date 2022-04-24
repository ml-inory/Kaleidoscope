#include <string>
#include <memory>
<<<<<<< HEAD
=======
#include <vector>

#pragma once
>>>>>>> 8896d9c5fffd6cefd7511a0e578d35eed69499e8

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
<<<<<<< HEAD
    NumberExprAST(double Val): Val(Val) {}

    double Val;
};

// 变量
class VariableExprAST : public ExprAST
=======
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS))
    {}
};

// 函数调用
class CallExprAST : public ExprAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string& Callee, std::vector<std::unique_ptr<ExprAST>> Args):
    Callee(Callee),
    Args(std::move(Args))
    {}
};

// 函数原型
class PrototypeExprAST : public ExprAST
>>>>>>> 8896d9c5fffd6cefd7511a0e578d35eed69499e8
{
    std::string Name;
    std::vector<std::string> Args;

public:
<<<<<<< HEAD
    VariableExprAST(const std::string& Name): Name(Name) {}

    std::string Name;
};

// 二目运算符
class BinaryExprAST : public ExprAST
{
public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> )
=======
    PrototypeExprAST(const std::string& Name, const std::vector<std::string>& Args):
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
>>>>>>> 8896d9c5fffd6cefd7511a0e578d35eed69499e8
};