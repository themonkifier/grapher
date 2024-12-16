#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

namespace errors {

class malformed_expression : public std::runtime_error {
public:
    malformed_expression()
        : std::runtime_error("malformed expression")
    {}

    malformed_expression(std::string expr)
        : std::runtime_error("malformed expression: " + expr)
    {}

    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
};

class empty : public std::runtime_error {
public:
    empty()
        : std::runtime_error("")
    {}

    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
};

class non_square_matrix : public std::runtime_error {
public:
    non_square_matrix()
        : std::runtime_error("matrix must be square")
    {}
    
    non_square_matrix(std::string function)
        : std::runtime_error("cannot compute " + function + " of a non-square matrix")
    {}

    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
};

}  // namespace errors

#endif  // ERRORS_H
