#ifndef LEXER_H
#define LEXER_H

#include "util/functions.h"

class Lexer {
public:
    struct Token {
        std::variant<std::monostate, complex, Matrix*, FuncInfo, std::string> data;
        enum class Tokens {
            NUMBER = 'n',
            FUNCTION = 'f',
            VARIABLE = 'v',

            EQUALS = '=',

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

            STREAM_END = '\0',
        } tokenType;

        Token(Tokens tokenType);
        Token(complex num);
        Token(FuncInfo func_info);
        Token(std::string variable_name);

        size_t operator()(const Token& t) const noexcept;
        std::string to_string() const noexcept;
    };

    Lexer(std::string input);
    std::vector<Token>& get_tokens();
    bool is_expression() const noexcept { return expression; }

private:
    void get_next() noexcept;
    char peek() const noexcept;
    bool is_negative_sign() const noexcept;
    Token generate_number();
    Token generate_function();
    std::string to_string() const noexcept;

    static bool isbrace(const char c) noexcept;

    std::vector<Token> tokens;
    std::string input;
    size_t pos{0};
    bool expression{false};
    char curr_input{'\0'};
    char prev_input{'\0'};
};

template<> struct std::hash<Lexer::Token> {
    std::size_t operator()(const Lexer::Token& t) const noexcept {
        return 904982468069 * static_cast<char>(t.tokenType) + 830434557313;
    }
};

#endif
