#include "parser.h"
#include "ast.h"
#include "util/errors.h"
#include "lexer.h"

using enum Lexer::Token::Tokens;
using std::vector;
using std::unordered_set;
using std::unique_ptr;

unordered_set<Lexer::Token::Tokens> TermItems({ ADD, SUBTRACT });
unordered_set<Lexer::Token::Tokens> FactorItems({ MULTIPLY, DIVIDE, MODULO });
unordered_set<Lexer::Token::Tokens> ExponentiateItems({ EXPONENTIATE }); // wow!

Parser::Parser(vector<Lexer::Token>& tokens)
    : tokens(tokens), pos(tokens.begin()), curr_token(nullptr)
{
    Get_Next();
}

Parser::Parser(vector<Lexer::Token>& tokens, vector<Lexer::Token>::iterator start)
    : tokens(tokens), pos(start), curr_token(&*start)
{
    Get_Next();
}

unique_ptr<AST::Node> Parser::Parse_Exp() {
    auto result = Factor();

    while (curr_token->tokenType != STREAM_END && result != nullptr && TermItems.count(curr_token->tokenType) > 0) {
        switch (curr_token->tokenType) {
            case ADD:
                Get_Next();
                result = std::make_unique<AST::Plus>(std::move(result), Factor());
                break;
            case SUBTRACT:
                Get_Next();
                result = std::make_unique<AST::Minus>(std::move(result), Factor());
                break;
            default:
                throw errors::malformed_expression();
        }
    }

    return result;
}

unique_ptr<AST::Node> Parser::Factor() {
    auto factor = Exponent();

    while (curr_token->tokenType != STREAM_END && factor != nullptr && FactorItems.count(curr_token->tokenType) > 0) {
        switch (curr_token->tokenType) {
            case MULTIPLY:
                Get_Next();
                factor = std::make_unique<AST::Times>(std::move(factor), Exponent());
                break;
            case DIVIDE:
                Get_Next();
                factor = std::make_unique<AST::Divide>(std::move(factor), Exponent());
                break;
            case MODULO:
                Get_Next();
                factor = std::make_unique<AST::Modulo>(std::move(factor), Exponent());
                break;
            default:
                throw errors::malformed_expression();
        }
    }

    return factor;
}

unique_ptr<AST::Node> Parser::Exponent() {
    auto exponent = Term();

    while (curr_token->tokenType != STREAM_END && exponent != nullptr && ExponentiateItems.count(curr_token->tokenType) > 0) {
        switch (curr_token->tokenType) {
            case EXPONENTIATE: {
                Get_Next();
                exponent = std::make_unique<AST::Exponentiate>(std::move(exponent), Term());
                break;
           }
            default:
                throw errors::malformed_expression();
        }
    }

    return exponent;
}

unique_ptr<AST::Node> Parser::Term() {
    unique_ptr<AST::Node> term = nullptr;

    switch (curr_token->tokenType) {
        case FUNCTION: {
            FuncInfo& func = std::get<FuncInfo>(curr_token->data);
            Get_Next(); // either number or '('

            if (curr_token->tokenType == STREAM_END) {
                throw errors::malformed_expression("expression ends in a function call");
            } else if (curr_token->tokenType != OPEN_PAREN) {
                // this assumes that the function being called is acting on a number, as far as i can tell?
                term = std::make_unique<AST::Number>(
                    std::get<complex>(func(std::get<complex>(curr_token->data)))
                );
                break;
            }

            Get_Next(); // start of inner expression, since previous token was '('
            term = std::make_unique<AST::Function>(func, Parse_Exp());
            break;
        }
        case OPEN_BRACE:
            Get_Next();
            term = Parse_Exp();
            break;
        case OPEN_PAREN:
            Get_Next();
            term = Parse_Exp();
            break;
        case NUMBER:
            term = std::make_unique<AST::Number>(std::get<complex>(curr_token->data));
            break;
        case VARIABLE:
            term = std::make_unique<AST::Variable>(std::get<std::string>(curr_token->data));
            break;
        default:
            throw errors::malformed_expression();
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
