#ifndef LEXER_H
#define LEXER_H

#include "functions.hh"

enum class Tokens {
    NUMBER = 'n',
    FUNCTION = 'f',

    ADD = '+',
    SUBTRACT = '-',
    MULTIPLY = '*',
    DIVIDE = '/',
    MODULO = '%',
    EXPONENTIATE = '^',

    OPEN_PAREN = '(',
    OPEN_SQUARE = '[',
    OPEN_BRACE = '{',
    CLOSE_PAREN = ')',
    CLOSE_SQUARE = ']',
    CLOSE_BRACE = '}',

    STREAM_END = '\0'
};

class Token {
public:
    Tokens tokenType;
    optional<complex> num;
    optional<FuncInfo> func_info;

    Token(Tokens tokenType, complex num = {});
    Token(FuncInfo func_info);
    
    size_t operator()(const Token& t) const noexcept;
    string to_string();
};

string to_string(complex num);

template<> struct std::hash<Token> {
    std::size_t operator()(const Token& t) const noexcept {
        return 904982468069 * static_cast<char>(t.tokenType) + 830434557313;
    }
};

class Lexer {
public:
    vector<Token> tokens;
    string input;
    size_t pos;
    char curr_input;

    Lexer(string input);
    void push(Token& token);
    void get_next();
    vector<Token>* get_tokens();
    Token generate_number();
    Token generate_function();

    string to_string();
};

#endif
