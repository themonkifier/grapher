#include "context.h"
#include "functions.h"
#include "../matrix/matrix.h"

Context::Context()
    : variables{}
{}

Variable& Context::operator[](std::string var) {
    return variables[var];
}

Variable& Context::at(std::string var) {
    return variables.at(var);
}

const Variable& Context::at(std::string var) const {
    return variables.at(var);
}

void Context::reset(std::string name, Variable value) {
    variables[name] = value;
}
