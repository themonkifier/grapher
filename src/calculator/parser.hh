#ifndef PARSER_HH
#define PARSER_HH

#include "lexer.hh"
#include "ast.hh"

class Parser {
public:
    Parser(vector<Token>& tokens);

    unique_ptr<AST> Parse_Exp();
    unique_ptr<AST> Factor();
    unique_ptr<AST> Exponent();
    unique_ptr<AST> Term();
private:
    vector<Token> tokens;
    vector<Token>::iterator pos;
    Token* curr_token;

    void Get_Next();
};

#endif
