#include "parser.hh"
#include "ast.hh"
#include "lexer.hh"
#include <stdexcept>
#include <iostream>

unordered_set<Tokens> TermItems({ Tokens::ADD, Tokens::SUBTRACT });
unordered_set<Tokens> FactorItems({ Tokens::MULTIPLY, Tokens::DIVIDE, Tokens::MODULO });
unordered_set<Tokens> ExponentiateItems({ Tokens::EXPONENTIATE }); // wow!

Parser::Parser(vector<Token>& tokens)
    : tokens(tokens), pos(tokens.begin()), curr_token(nullptr)
{
    Get_Next();
}

unique_ptr<AST> Parser::Parse_Exp() {
    unique_ptr<AST> result = Factor();

    while (curr_token->tokenType != Tokens::STREAM_END && result != nullptr && TermItems.count(curr_token->tokenType) > 0) {
        switch (curr_token->tokenType) {
            case Tokens::ADD:
                Get_Next();
                result = std::make_unique<ASTAdd>(std::move(result), Factor());
                break;
            case Tokens::SUBTRACT:
                Get_Next();
                result = std::make_unique<ASTSubtract>(std::move(result), Factor());
                break;
            default:
                throw std::runtime_error("malformed expression");
        }
    }

    return result;
}

unique_ptr<AST> Parser::Factor() {
    unique_ptr<AST> factor = Exponent();

    while (curr_token->tokenType != Tokens::STREAM_END && factor != nullptr && FactorItems.count(curr_token->tokenType) > 0) {
        switch (curr_token->tokenType) {
            case Tokens::MULTIPLY:
                Get_Next();
                factor = std::make_unique<ASTMultiply>(std::move(factor), Exponent());
                break;
            case Tokens::DIVIDE:
                Get_Next();
                factor = std::make_unique<ASTDivide>(std::move(factor), Exponent());
                break;
            case Tokens::MODULO:
                Get_Next();
                factor = std::make_unique<ASTModulo>(std::move(factor), Exponent());
                break;
            default:
                throw std::runtime_error("malformed expression");
        }
    }

    return factor;
}

unique_ptr<AST> Parser::Exponent() {
    unique_ptr<AST> exponent = Term();

    while (curr_token->tokenType != Tokens::STREAM_END && exponent != nullptr && ExponentiateItems.count(curr_token->tokenType) > 0) {
        if (curr_token->tokenType == Tokens::EXPONENTIATE) {
            Get_Next();
            exponent = std::make_unique<ASTExponentiate>(std::move(exponent), Term());
        } else {
            throw std::runtime_error("malformed expression");
        }
    }

    return exponent;
}

unique_ptr<AST> Parser::Term() {
    unique_ptr<AST> term = nullptr;

    switch (curr_token->tokenType) {
        case Tokens::FUNCTION: {
            FuncInfo& func_info = curr_token->func_info.value();
            Get_Next(); /* either number or '(' */

            if (curr_token->tokenType != Tokens::OPEN_PAREN) {
                term = std::make_unique<ASTLeaf>(func_info.second(curr_token->num.value()));
                break;
            }

            Get_Next(); /* start of inner expression, since previous token was '(' */
            term = std::make_unique<ASTFunction>(func_info, Parse_Exp());

            std::cout << (pos - 1)->to_string() << std::endl;

            if (curr_token->tokenType != Tokens::CLOSE_PAREN) {
                throw std::runtime_error("missing ) after function call " + term->to_string() + " " + curr_token->to_string());
            }
            break;
        }
        case Tokens::OPEN_BRACE:;
            Get_Next();
            term = Parse_Exp();
            if (curr_token->tokenType != Tokens::CLOSE_BRACE) {
                throw std::runtime_error("missing }");
            }
            break;
        case Tokens::OPEN_PAREN:
            Get_Next();
            term = Parse_Exp();
            if (curr_token->tokenType != Tokens::CLOSE_PAREN) {
                throw std::runtime_error("missing )");
            }
            break;
        case Tokens::NUMBER:
            term = std::make_unique<ASTLeaf>(curr_token->num.value());
            break;
        default:
            throw std::runtime_error("malformed expression");
    }

    Get_Next();

    return term;
}

void Parser::Get_Next() {
    if (pos != tokens.end()) {
        curr_token = &*pos;
        pos++;
    }
}
