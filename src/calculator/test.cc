#include <assert.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <cstring>

#include "lexer.hh"
#include "ast.hh"
#include "parser.hh"

int tests, passed;

#define assert_equal(EXPRESSION_1, EXPRESSION_2) {                      \
    tests++;                                                            \
    bool eql = EXPRESSION_1 == EXPRESSION_2;                            \
    if (!eql) {                                                         \
        std::cout << #EXPRESSION_1 " != " #EXPRESSION_2 << std::endl;   \
    } else {                                                            \
        passed++;                                                       \
    }                                                                   \
}

#define assert_not_equal(EXPRESSION_1, EXPRESSION_2) {                  \
    tests++;                                                            \
    bool eql = EXPRESSION_1 == EXPRESSION_2;                            \
    if (eql) {                                                          \
        std::cout << #EXPRESSION_1 " == " #EXPRESSION_2 << std::endl;   \
    } else {                                                            \
        passed++;                                                       \
    }                                                                   \
}

#define assert_throws(EXPRESSION, EXCEPTION, WHAT) {                    \
    tests++;                                                            \
    try {                                                               \
        EXPRESSION;                                                     \
        std::cout << #EXPRESSION " didn't throw " #WHAT << std::endl;   \
    } catch (const EXCEPTION& e) {                                      \
        if (std::strcmp(e.what(), WHAT) == 0)                           \
        {                                                               \
            passed++;                                                   \
        }                                                               \
    }                                                                   \
}


#define assert_empty(EXPRESSION) {                              \
    tests++;                                                    \
    bool empty = EXPRESSION.size() == 0;                        \
    if (!empty) {                                               \
        std::cout << #EXPRESSION ".size() != 0" << std::endl;   \
    } else {                                                    \
        passed++;                                               \
    }                                                           \
}

#define assert_not_empty(EXPRESSION) {                          \
    tests++;                                                    \
    bool empty = EXPRESSION.size() == 0;                        \
    if (empty) {                                                \
        std::cout << #EXPRESSION ".size() == 0" << std::endl;   \
    } else {                                                    \
        passed++;                                               \
    }                                                           \
}

#define assert_null(EXPRESSION) {                               \
    tests++;                                                    \
    bool null = EXPRESSION == nullptr;                          \
    if (!null) {                                                \
        std::cout << #EXPRESSION " != nullptr" << std::endl;    \
    } else {                                                    \
        passed++;                                               \
    }                                                           \
}

#define assert_not_null(EXPRESSION) {                           \
    tests++;                                                    \
    bool null = EXPRESSION == nullptr;                          \
    if (null) {                                                 \
        std::cout << #EXPRESSION " == nullptr" << std::endl;    \
    } else {                                                    \
        passed++;                                               \
    }                                                           \
}

void lexer_all_tokens();
void lexer_invalid_decimal();
void lexer_invalid_character();

void ast_string();
void ast_term();
void ast_factor();
void ast_braces();
void ast_missing_brace();

void eval_trig();
void eval_complex_identity();
void eval_integral();

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    lexer_all_tokens();
    lexer_invalid_decimal();
    lexer_invalid_character();

    ast_string();
    ast_term();
    ast_factor();
    ast_braces();
    ast_missing_brace();

    eval_trig();
    eval_complex_identity();
    eval_integral();

    if (tests != passed) {
        std::cout << passed << "/" << tests << "passed (" << ((double) passed) / tests * 100 << "%)" << std::endl;
    } else {
        std::cout << "All tests passed (" << ((double) passed) / tests * 100 << "%)" << std::endl;
    }
}

void lexer_all_tokens() {
    Lexer lexer("( ) 4646 + - * / 565.788");
    vector<Token> tokens = *( lexer.get_tokens() );

    assert_not_empty(tokens);

    assert_equal(static_cast<char>(tokens[0].tokenType), static_cast<char>(Tokens::OPEN_PAREN));

    assert_equal(static_cast<char>(tokens[2].tokenType), static_cast<char>(Tokens::NUMBER));
    assert_equal(tokens[2].num.value(), complex(4646));

    assert_equal(static_cast<char>(tokens[4].tokenType), static_cast<char>(Tokens::SUBTRACT));

    assert_equal(static_cast<char>(tokens[7].tokenType), static_cast<char>(Tokens::NUMBER));
    // assert_equal_fp(tokens[7].num.value().real(), 565.788);
}
void lexer_invalid_decimal() {
    Lexer lexer("hello world");
    assert_throws(lexer.get_tokens(), std::runtime_error, "unknown string: hello");

}
void lexer_invalid_character() {
    Lexer lexer("127.0.0.0");
    assert_throws(lexer.get_tokens(), std::runtime_error, "invalid format: too many '.'s");
}

void ast_string() {
    string expected = "((56 - (64 / 8)) + 4)";

    Lexer lexer("56    - 64/8 +4");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    // assert_equal(expected, ast->to_string());
}

void ast_term() {
    complex expected = 45;

    Lexer lexer("25+25 -5");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    assert_equal(expected, ast->eval());
}

void ast_factor() {
    complex expected = 5.5;

    Lexer lexer("121/11*0.5");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    assert_equal(expected, ast->eval());
}

void ast_braces() {
    complex expected = 0;

    Lexer lexer("10-5*(25/5)+15");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    assert_equal(expected, ast->eval());
}

void ast_missing_brace() {
    Lexer lexer("(25/5");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    assert_throws(parser.Parse_Exp(), std::runtime_error, "missing )");
}

void eval_trig() {
    complex expected = 2;
    Lexer lexer("sin(pi / 2) + tan(pi) - cos(3pi)");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    assert_equal(expected, ast->eval());
}

void eval_complex_identity() {
    complex expected = -1;
    Lexer lexer("e^(i*pi)");
    vector<Token> tokens = *( lexer.get_tokens() );
    assert_not_empty(tokens);

    Parser parser(tokens);
    unique_ptr<AST> ast = parser.Parse_Exp();
    assert_not_null(ast);

    assert_equal(expected, ast->eval());
}

void eval_integral() {
    ;
}
