#include "ast.hh"
#include <cmath>

ASTLeaf::ASTLeaf(complex num) 
    : num(num)
{}

complex ASTLeaf::eval() {
    return num;
}

std::string ASTLeaf::to_string() {
    if (num.imag() == 0) {
        return std::to_string(num.real());
    } else if (num.real() == 0) {
        return (num.imag() < 0 ? " + " : " - ") +
            std::to_string(std::abs(num.imag())) + "i";
    }
    return std::to_string(num.real()) +
        (num.imag() < 0 ? " + " : " - ") +
        std::to_string(std::abs(num.imag())) + "i";
}

ASTAdd::ASTAdd(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex ASTAdd::eval() {
    return leftNode->eval() + rightNode->eval();
}

std::string ASTAdd::to_string() {
    return "(" + leftNode->to_string() + " + " + rightNode->to_string() + ")";
}

ASTSubtract::ASTSubtract(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex ASTSubtract::eval()  {
    return leftNode->eval() - rightNode->eval();
}

std::string ASTSubtract::to_string()  {
    return "(" + leftNode->to_string() + " - " + rightNode->to_string() + ")";
}

ASTMultiply::ASTMultiply(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex ASTMultiply::eval()  {
    return leftNode->eval() * rightNode->eval();
}

std::string ASTMultiply::to_string()  {
    return "(" + leftNode->to_string() + " * " + rightNode->to_string() + ")";
}

ASTDivide::ASTDivide(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex ASTDivide::eval()  {
    return leftNode->eval() / rightNode->eval();
}

std::string ASTDivide::to_string()  {
    return "(" + leftNode->to_string() + " / " + rightNode->to_string() + ")";
}

ASTModulo::ASTModulo(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex mod(complex a, complex b) {
    complex x = a / b;
    x = complex(std::floor(x.real()), floor(x.imag()));

    complex z = x * b;

    return a - z;
}

complex ASTModulo::eval()  {
    return mod(leftNode->eval(), rightNode->eval());
}

std::string ASTModulo::to_string()  {
    return "(" + leftNode->to_string() + " % " + rightNode->to_string() + ")";
}

ASTExponentiate::ASTExponentiate(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode)
    : leftNode(std::move(leftNode)), rightNode(std::move(rightNode))
{}

complex ASTExponentiate::eval()  {
    return std::pow(leftNode->eval(), rightNode->eval());
}

std::string ASTExponentiate::to_string()  {
    return "(" + leftNode->to_string() + " ^ " + rightNode->to_string() + ")";
}

ASTFunction::ASTFunction(FuncInfo func_info, unique_ptr<AST> node)
    : func_info(func_info), node(std::move(node))
{}

complex ASTFunction::eval() {
    return func_info.second(node->eval());
}

std::string ASTFunction::to_string() {
    return func_info.first + "(" + node->to_string() + ")";
}
