#include "lexer.h"
#include "util/errors.h"
#include "matrix/matrix.h"

using enum Lexer::Token::Tokens;

Lexer::Token::Token(Lexer::Token::Tokens tokenType)
    : tokenType(tokenType)
{}

Lexer::Token::Token(complex num)
    : tokenType(NUMBER)
{
    data = num;
}

Lexer::Token::Token(FuncInfo func_info)
    : tokenType(FUNCTION)
{
    data = func_info;
}

Lexer::Token::Token(std::string variable_name)
    : tokenType(VARIABLE)
{
    data = variable_name;
}

std::string Lexer::Token::to_string() const noexcept {
    switch (tokenType) {
    case NUMBER:
        return ::to_string(std::get<complex>(data));
    case FUNCTION:
        return std::get<FuncInfo>(data).name;
    case VARIABLE:
        return std::get<std::string>(data);
    case STREAM_END:
        return "{EOF}";
    default:
        return { static_cast<char>(tokenType) };
    }
}

size_t Lexer::Token::operator()(const Token& t) const noexcept {
    return 904982468069 * static_cast<char>(t.tokenType) + 830434557313;
}

Lexer::Lexer(std::string input)
    : tokens{}, input(input)
{
    if (input.size() > 0) {
        curr_input = input[0];
    } else {
        curr_input = '\0';
    }
}

void Lexer::get_next() noexcept {
    if (!std::iscntrl(curr_input) && !std::isspace(curr_input)) {
        prev_input = curr_input;
    }

    if (pos < input.size() - 1) {
        pos++;
        curr_input = input[pos];
    } else {
        curr_input = '\0';
    }
}

char Lexer::peek() const noexcept {
    if (pos < input.size() - 1) {
        return input[pos + 1];
    } else {
        return '\0';
    }
}

std::vector<Lexer::Token>& Lexer::get_tokens() {
    while (true) {
        if (numbers.count(curr_input) > 0 || is_negative_sign()) {
            tokens.push_back(generate_number());
        } else if (std::isalpha(curr_input)) {
            tokens.push_back(generate_function());
        } else if (curr_input == '\0') {
            tokens.push_back({STREAM_END});
            break;
        } else {
            switch (curr_input) {
            case ' ':
            case '\t':
                get_next();
                continue;

            case '=':
                if (expression) {
                    throw errors::malformed_expression("too many '='s");
                }
                expression = true;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
                tokens.push_back({Token::Tokens(curr_input)});
                get_next();
                break;

            default:
                throw errors::malformed_expression("operator " + std::string{curr_input});
            }
        }
    }

    return tokens;
}

bool Lexer::is_negative_sign() const noexcept {
    return curr_input == '-' && (prev_input == '\0' || isbrace(prev_input));
}

Lexer::Token Lexer::generate_number() {
    int decimal_count = 0;
    std::string builder;

    if (curr_input == '-') {
        builder += curr_input;
        get_next();
    }

    while (numbers.count(curr_input) > 0) {
        if (curr_input == '.' && decimal_count == 0) {
            decimal_count++;
        } else if (curr_input == '.') {
            throw errors::malformed_expression("too many '.'s");
        }

        if (builder.size() < 1 && decimal_count > 0) {
            builder += '0';
        }

        builder += curr_input;
        get_next();
    }

    if (curr_input == 'i') {
        get_next();
        if (builder.size() == 0 || (builder.size() == 1 && builder[0] == '-')) {
            builder += "1";
        }

        return { complex(0, std::stof(builder)) };
    }

    try {
        return { std::stof(builder) };
    } catch (const std::invalid_argument& e) {
        throw errors::malformed_expression{};
    }
}

Lexer::Token Lexer::generate_function() {
    std::string builder;

    while (std::isalpha(curr_input) || curr_input == '_') {
        builder += curr_input;
        get_next();
    }

    if (functions.count(builder) > 0) {
        return { FuncInfo{ builder, functions.at(builder) } };
    } else if (matrix_ops.count(builder) > 0) {
        return { FuncInfo{ builder, matrix_ops.at(builder) } };
    } else if (matrix_ops_.count(builder) > 0) {
        return { FuncInfo{ builder, matrix_ops_.at(builder) } };
    } else if (constants.count(builder) > 0) {
        // this directly puts in the numeric value of the constant,
        //  is this something we want? or do we want to save it for
        //  later like with variables?
        return { constants.at(builder) };
    } else if (builder == "mod" || builder == "modulo") {
        return { MODULO };
    } else if (!expression) {
        return { builder };
    }

    throw errors::malformed_expression("string \"" + builder + "\"");
}

std::string Lexer::to_string() const noexcept {
    std::string ret;

    for (auto& token : tokens) {
        ret += token.to_string();
    }

    return ret;
}

bool Lexer::isbrace(const char c) noexcept {
    return (c == '(' || c == '[' || c == '{') || (c == ')' || c ==']' || c == '}');
}
