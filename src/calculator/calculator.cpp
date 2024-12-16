#include "calculator.h"
#include "util/errors.h"
#include "parser.h"

using std::vector;
using std::string;

Calculator::Calculator() noexcept
{}

#include <iostream>
Variable Calculator::eval(string input, ContextPtr context) {
    if (input.size() == 0) {
        throw errors::empty();
    }

    Lexer lexer{input};

    vector<Lexer::Token> tokens = lexer.get_tokens();
    for (const auto& token : tokens) {
        std::cout << token.to_string() << ' ';
    }
    std::cout << std::endl;

    if (lexer.is_expression()) {
        if (!valid_expression(tokens)) {
            throw errors::malformed_expression();
        }

        std::string name = std::get<std::string>(tokens[0].data);

        Parser parser{tokens, std::next(tokens.begin(), 2)};
        auto ast = parser.Parse_Exp();

        context->reset(name, ast->eval(context));
        reset(name);

        return context->at(variable);
    }
    Parser parser{tokens};
    auto ast = parser.Parse_Exp();

    if (ast == nullptr) {
        throw errors::malformed_expression(input);
    }

    return ast->eval(context);
}

void Calculator::reset(string name) {
    variable = name;
}

bool Calculator::valid_expression(const vector<Lexer::Token>& tokens) const {
    for (auto it = tokens.begin() + 1; it != tokens.end(); ++it) {
        const auto& token = *it;
        if (token.tokenType == Lexer::Token::Tokens::VARIABLE && std::get<std::string>(token.data) == variable) {
            throw errors::malformed_expression("no recursive definitions");
        }
    }
    return tokens.size() >= 3 &&
           tokens[0].tokenType == Lexer::Token::Tokens::VARIABLE &&
           tokens[1].tokenType == Lexer::Token::Tokens::EQUALS;
}

