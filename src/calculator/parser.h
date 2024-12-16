#ifndef PARSER_HH
#define PARSER_HH

#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser(std::vector<Lexer::Token>& tokens);
    Parser(std::vector<Lexer::Token>& tokens, std::vector<Lexer::Token>::iterator start);

    std::unique_ptr<AST::Node> Parse_Exp();
    std::unique_ptr<AST::Node> Factor();
    std::unique_ptr<AST::Node> Exponent();
    std::unique_ptr<AST::Node> Term();
private:
    std::vector<Lexer::Token> tokens;
    std::vector<Lexer::Token>::iterator pos;
    Lexer::Token* curr_token;

    void Get_Next();
};

#endif
