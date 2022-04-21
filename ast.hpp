#include <string>

class ExprAST
{
public:
    virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST
{
public:
    NumberExprAST(double val): m_val(val)
    {}

private:
    double m_val;
};

class VariableExprAST : public ExprAST
{
public:
    VariableExprAST(const std::string& name): m_name(name)
    {}

private:
    std::string m_name;
};