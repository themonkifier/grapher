#ifndef MATRIX_H
#define MATRIX_H

#include "../util/functions.h"
#include "../util/numerical.h"
#include "../util/errors.h"
#include "../calculator.h"

#include <string>
#include <vector>
#include <optional>
#include <memory>

using std::size_t;
#include "../util/context.h"
#include "../calculator.h"

class Matrix {
public:
    using column_type = std::vector<complex>;

    explicit Matrix(size_t rows, size_t columns, ContextPtr context);
    explicit Matrix(complex num, ContextPtr context);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix() {}

    column_type& operator[](size_t row) { return arr[row]; }
    const column_type& operator[](size_t row) const { return arr[row]; }

    size_t get_rows() const noexcept { return rows; }
    size_t get_columns() const noexcept { return columns; }

    complex set(size_t row, size_t column, std::string new_input);
    complex get() const;

    complex determinant() const;
    complex trace() const;
    std::shared_ptr<Matrix> inverse() const;
    std::shared_ptr<Matrix> transpose() const;
    std::shared_ptr<Matrix> rref() const;

    Matrix& operator+=(const Matrix& rhs);
    friend Matrix operator+(Matrix lhs, const Matrix& rhs);
    Matrix& operator-=(const Matrix& rhs);
    friend Matrix operator-(Matrix lhs, const Matrix& rhs);
    Matrix& operator*=(const complex& rhs);
    friend Matrix operator*(Matrix lhs, const complex& rhs);
    Matrix& operator/=(const complex& rhs);
    friend Matrix operator/(Matrix lhs, const complex& rhs);
    Matrix& operator*=(const Matrix& rhs);
    friend Matrix operator*(Matrix lhs, const Matrix& rhs);

    std::string to_string() const;
    void reset(std::string name, Variable value);
    void resize(size_t rows, size_t columns);

protected:
    mutable struct Cache {
        std::optional<complex> determinant;
        std::optional<complex> trace;
        std::shared_ptr<Matrix> inverse;
        std::shared_ptr<Matrix> transpose;
        std::shared_ptr<Matrix> rref;

        //std::shared_ptr<Matrix> ei;
        //std::shared_ptr<Matrix> g;
        //std::shared_ptr<Matrix> en;

        //std::shared_ptr<Matrix> q;
        //std::shared_ptr<Matrix> r;

        //std::shared_ptr<Matrix> l;
        //std::shared_ptr<Matrix> u;

        //std::shared_ptr<Matrix> s;
        //std::shared_ptr<Matrix> v;
        //std::shared_ptr<Matrix> d;
    } cache;

    size_t rows;
    size_t columns;
    Calculator calculator;
    ContextPtr context;
    std::vector<column_type> arr;

    Matrix minor(size_t row, size_t column) const;
    Matrix cofactor() const;
    Matrix adjoint() const;
};  // class Matrix

#endif  // MATRIX_H
