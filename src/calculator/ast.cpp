#include "ast.h"
#include "util/errors.h"
#include <cmath>

using std::unique_ptr;

namespace AST {

Number::Number(complex num)
    : num(num)
{}

std::string Number::to_string() {
    return ::to_string(num);
}

Variable::Variable(std::string name)
    : name{name}
{}

::Variable Variable::eval(ContextPtr context) {
    try {
        return context->at(name);
    } catch (const std::out_of_range& e) {
        throw errors::malformed_expression("unknown variable '" + name + "'");
    }
}

std::string Variable::to_string() {
    return name;
}

Function::Function(FuncInfo func, unique_ptr<Node> node)
    : func(func), node(std::move(node))
{}

::Variable Function::eval(ContextPtr context) {
    ::Variable input = node->eval(context);
    return func(input);
}

std::string Function::to_string() {
    return func.name + "(" + node->to_string() + ")";
}

Operator::Operator(unique_ptr<Node> left, unique_ptr<Node> right, char op)
    : left(std::move(left)), right(std::move(right)), op(op)
{}

std::string Operator::to_string() {
    return "(" + left->to_string() + op + right->to_string() + ")";
}

Plus::Plus(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '+')
{}

::Variable Plus::eval(ContextPtr context) {
    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 0 && r.index() == 0) {
        return std::get<complex>(l) + std::get<complex>(r);
    } else if (l.index() == 1 && r.index() == 1) {
        return std::get<Matrix>(l) + std::get<Matrix>(r);
    } else {
        throw errors::malformed_expression("trying to add a matrix and scalar value");
    }
}

Minus::Minus(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '-')
{}

::Variable Minus::eval(ContextPtr context) {
    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 0 && r.index() == 0) {
        return std::get<complex>(l) - std::get<complex>(r);
    } else if (l.index() == 1 && r.index() == 1) {
        return std::get<Matrix>(l) - std::get<Matrix>(r);
    } else {
        throw errors::malformed_expression("trying to subtract a matrix and scalar value");
    }
}

Times::Times(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '*')
{}

::Variable Times::eval(ContextPtr context) {
    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 0 && r.index() == 0) {
        return std::get<complex>(l) * std::get<complex>(r);
    } else if (l.index() == 1 && r.index() == 1) {
        return std::get<Matrix>(l) * std::get<Matrix>(r);
    } else if (l.index() == 0 && r.index() == 1) {
        return std::get<Matrix>(r) * std::get<complex>(l);
    } else {
        return std::get<Matrix>(l) * std::get<complex>(r);
    }
}

Divide::Divide(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '/')
{}

::Variable Divide::eval(ContextPtr context) {
    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 0 && r.index() == 0) {
        return std::get<complex>(l) / std::get<complex>(r);
    } else if (l.index() == 1 && r.index() == 0) {
        return std::get<Matrix>(l) / std::get<complex>(r);
    } else {
        throw errors::malformed_expression("trying to divide by a matrix");
    }
}

Modulo::Modulo(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '%')
{}

::Variable Modulo::eval(ContextPtr context) {
    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 0 && r.index() == 0) {
        return std::get<complex>(l) % std::get<complex>(r);
    //} else if (l.index() == 1 && r.index() == 0) {
        //return std::get<Matrix>(l) % std::get<complex>(r);
    } else {
        throw errors::malformed_expression("trying to modulo by a matrix");
    }
}

Exponentiate::Exponentiate(unique_ptr<Node> left, unique_ptr<Node> right)
    : Operator(std::move(left), std::move(right), '^')
{}

::Variable Exponentiate::eval(ContextPtr context)  {
    /* see treerotation.md */

    if ( dynamic_cast<Exponentiate*>(left.get()) ) {
        auto temp = static_cast<Exponentiate*>(left.release());
        left = std::move(temp->left);

        temp->left = std::move(temp->right);
        temp->right = std::move(right);
        right = unique_ptr<Node>(temp);
    }

    ::Variable l = left->eval(context), r = right->eval(context);
    if (l.index() == 1 || r.index() == 1) {
        throw errors::malformed_expression("trying to use a matrix in an exponential");
    }

    return std::pow(std::get<complex>(l), std::get<complex>(r));
}

}  // namespace AST
