#include "lexer.hh"

Token::Token(Tokens tokenType, complex num)
    : tokenType(tokenType), num{num}, func_info{}
{}

Token::Token(FuncInfo func_info)
    : tokenType(Tokens::FUNCTION), num{}, func_info{func_info}
{}

string Token::to_string() {
    switch (tokenType) {
        using enum Tokens;

        case NUMBER:
            if (num.value().imag() == 0) {
                return std::to_string(num.value().real());
            } else if (num.value().real() == 0) {
                return (num.value().imag() < 0 ? " + " : " - ") +
                    std::to_string(std::abs(num.value().imag())) + "i";
            }
            return std::to_string(num.value().real()) +
                (num.value().imag() < 0 ? " + " : " - ") +
                std::to_string(std::abs(num.value().imag())) + "i";
        case FUNCTION:
            return func_info->first;
        case STREAM_END:
            return "{EOF}";
        default:
            return { static_cast<char>(tokenType) };
    }
}

string to_string(complex num) {
    if (num.imag() == 0) {
        return std::to_string(num.real());
    } else if (num.real() == 0) {
        return (num.imag() < 0 ? " + " : " - ") +
            std::to_string(std::abs(num.imag())) + "i";
    }
    return std::to_string(num.real()) +
        (num.imag() < 0 ? " + " : " - ") +
        std::to_string(std::abs(num.imag())) + "i";
}

size_t Token::operator()(const Token& t) const noexcept {
    return 904982468069 * static_cast<char>(t.tokenType) + 830434557313;
}

Lexer::Lexer(string input)
    : tokens{}, input(input), pos(0)
{
    if (input.size() > 0) {
        curr_input = input[0];
    } else {
        curr_input = '\0';
    }
}

void Lexer::get_next() {
    if (pos < input.size() - 1) {
        pos++;
        curr_input = input[pos];
    } else {
        curr_input = '\0';
    }
}

vector<Token>* Lexer::get_tokens() {
    while (true) {
        if (numbers.count(curr_input) > 0) {
            tokens.push_back(generate_number());
        } else if (std::isalpha(curr_input)) {
            tokens.push_back(generate_function());
        } else if (curr_input == '\0') {
            tokens.push_back({Tokens::STREAM_END});
            break;
        } else {
            switch (curr_input) {
                case ' ':
                case '\t':
                get_next();
                continue;

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
                tokens.push_back({Tokens(curr_input)});
                get_next();
                break;

                default:
                throw std::runtime_error(string{curr_input} + " is an unsupported type");
            }
        }
    }

    return &tokens;
}

Token Lexer::generate_number() {
    int decimal_count = 0;
    string builder;

    while (numbers.count(curr_input) > 0) {
        if (curr_input == '.' && decimal_count == 0) {
            decimal_count++;
        } else if (curr_input == '.') {
            throw std::runtime_error("invalid format: too many '.'s");
        }

        if (builder.size() < 1 && decimal_count > 0) {
            builder += '0';
        }

        builder += curr_input;
        get_next();
    }

    if (curr_input == 'i') {
        get_next();
        return { Tokens::NUMBER, complex(0, -std::stof(builder)) };
    }

    return { Tokens::NUMBER, std::stof(builder) };
}

Token Lexer::generate_function() {
    string builder;

    while (std::isalpha(curr_input) || curr_input == '_') {
        builder += curr_input;
        get_next();
    }

    if (functions.count(builder) > 0) {
        return { { builder, functions.at(builder) } };
    } else if (constants.count(builder) > 0) {
        return { Tokens::NUMBER, constants.at(builder) };
    } else if (builder == "mod" || builder == "modulo") {
        return { Tokens::MODULO };
    }

    throw std::runtime_error("unknown string: " + builder);
}

string Lexer::to_string() {
    string ret;

    for (auto& token : tokens) {
        ret += token.to_string();
    }

    return ret;
}
