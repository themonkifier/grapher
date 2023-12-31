#ifndef AST_HH
#define AST_HH

#include <string>

#include "lexer.hh"

class AST {
public:
    AST() {}
    virtual ~AST() {}

    virtual complex eval() = 0;
    virtual std::string to_string() = 0;
};

class ASTLeaf : public AST {
public:
    complex num;
    ASTLeaf(complex num);

    complex eval() override;
    std::string to_string() override;
};

class ASTAdd : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTAdd(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTSubtract : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTSubtract(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTMultiply : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTMultiply(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTDivide : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTDivide(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTModulo : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTModulo(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTExponentiate : public AST {
public:
    unique_ptr<AST> leftNode;
    unique_ptr<AST> rightNode;

    ASTExponentiate(unique_ptr<AST> leftNode, unique_ptr<AST> rightNode);

    complex eval() override;
    std::string to_string() override;
};

class ASTFunction : public AST {
public:
    FuncInfo func_info;
    unique_ptr<AST> node;

    ASTFunction(FuncInfo func_info, unique_ptr<AST> node);

    complex eval() override;
    std::string to_string() override;
};

#endif
