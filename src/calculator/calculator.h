#ifndef CALCULATOR_HH
#define CALCULATOR_HH

#include "util/context.h"
#include "lexer.h"

#include <map>
#include <memory>

class Calculator {
public:
    explicit Calculator() noexcept;
    Variable eval(std::string input, ContextPtr context);
    void reset(std::string name);

private:
    std::string variable;

    bool valid_expression(const vector<Lexer::Token>& tokens) const ;
};

#endif
