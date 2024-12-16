#ifndef CONTEXT_H
#define CONTEXT_H

#include "numerical.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <complex>
#include <variant>

class Context {
public:
    Context();
    Context(std::unordered_map<std::string, Variable> variables);

    Variable& operator[](std::string var);
    //const Variable& operator[](std::string var) const;

    Variable& at(std::string var);
    const Variable& at(std::string var) const;

    void reset(std::string name, Variable value);

private:
    std::unordered_map<std::string, Variable> variables;
};

using ContextPtr = std::shared_ptr<Context>;

#endif // CONTEXT_H
