#ifndef AST_HH
#define AST_HH

#include "util/functions.h"
#include "matrix/matrix.h"
#include <memory>

namespace AST {

class Node {
public:
    Node() {}
    virtual ~Node() = default;
    virtual ::Variable eval(ContextPtr context) = 0;
    virtual std::string to_string() = 0;
};

class Leaf : public Node {};

class Number : public Leaf {
public:
    Number(complex num);
    ::Variable eval(ContextPtr context) override { return num; }
    std::string to_string() override;
private:
    complex num;
};

class Variable : public Leaf {
public:
    Variable(std::string name);
    ::Variable eval(ContextPtr context) override;
    std::string to_string() override;
private:
    std::string name;
};

class Function : public Node {
public:
    FuncInfo func;
    std::unique_ptr<Node> node;
    Function(FuncInfo func, std::unique_ptr<Node> node);
    ::Variable eval(ContextPtr context) override;
    std::string to_string() override;
};

class Operator : public Node {
public:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Operator(std::unique_ptr<Node> left, std::unique_ptr<Node> right, char op);
    std::string to_string() override;
private:
    char op;
};

class Plus : public Operator {
public:
    Plus(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

class Minus : public Operator {
public:
    Minus(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

class Times : public Operator {
public:
    Times(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

class Divide : public Operator {
public:
    Divide(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

class Modulo : public Operator {
public:
    Modulo(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

class Exponentiate : public Operator {
public:
    Exponentiate(std::unique_ptr<Node> left, std::unique_ptr<Node> right);
    ::Variable eval(ContextPtr context) override;
};

}  // namespace AST
#endif
