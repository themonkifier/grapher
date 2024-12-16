#include "matrix.h"

complex sign(size_t i, size_t j) {
    if ((i + j) % 2) {
        return -1;
    }

    return 1;
}

Matrix::Matrix(size_t rows, size_t columns, ContextPtr context)
    : rows{rows}, columns{columns}, context{context}
{
    arr.reserve(rows);
    for (int i = 0; i < rows; ++i) {
        arr.push_back(column_type(columns));
    }
}

Matrix::Matrix(complex num, ContextPtr context)
    : Matrix{1, 1, context}
{
    (*this)[0][0] = num;
}

Matrix::Matrix(const Matrix& other)
    : cache{other.cache},
      rows{other.rows},
      columns{other.columns},
      calculator{other.calculator},
      arr{other.arr}
{}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        cache = other.cache;
        rows = other.rows;
        columns = other.columns;
        calculator = other.calculator;
        arr = other.arr;
    }

    return *this;
}

complex Matrix::set(size_t row, size_t column, std::string input) {
    Variable result = calculator.eval(input, context);
    if (result.index() != 0) {
        throw errors::malformed_expression("cannot use matrix expression as matrix element");
    }

    auto& element = (*this)[row][column];
    element = std::get<complex>(result);
    return element;
}

complex Matrix::get() const {
    if (rows != 1 || columns != 1) {
        throw errors::malformed_expression("check the dimensions of this (matrix) equation");
    }

    return (*this)[0][0];
}

complex Matrix::determinant() const {
    if (rows != columns) {
        throw errors::non_square_matrix("determinant");
    }

    if (cache.determinant.has_value()) {
        return cache.determinant.value();
    }

    if (rows == 1) {
        return arr[0][0];
    } else if (rows == 2) {
        return arr[0][0] * arr[1][1] - arr[0][1] * arr[1][0];
    }

    complex det = 0;
    Matrix res{rows - 1, columns - 1, context};

    for (int i = 0; i < columns; i++) {
        det += arr[i][0] * sign(i, 0) * minor(i, 0).determinant();
    }

    cache.determinant = det;
    return det;
}

complex Matrix::trace() const {
    if (rows != columns) {
        throw errors::non_square_matrix("trace");
    }

    if (cache.trace.has_value()) {
        return cache.trace.value();
    }

    complex tr = 0;
    for (size_t i = 0; i < rows; ++i) {
        tr += arr[i][i];
    }

    cache.trace = tr;
    return tr;
}

std::shared_ptr<Matrix> Matrix::inverse() const {
    if (rows != columns) {
        throw errors::non_square_matrix("inverse");
    }

    if (cache.inverse) {
        return cache.inverse;
    }

    auto inv = std::make_shared<Matrix>(adjoint() / determinant());
    cache.inverse = inv;
    return inv;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
    if (rows != rhs.rows || columns != rhs.columns) {
        throw errors::malformed_expression("the two matrices must have the same size");
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            arr[i][j] += rhs[i][j];
        }
    }

    return *this;
}

Matrix operator+(Matrix lhs, const Matrix& rhs) {
    lhs += rhs;
    return lhs;
}

Matrix& Matrix::operator-=(const Matrix& rhs) {
    if (rows != rhs.rows || columns != rhs.columns) {
        throw errors::malformed_expression("the two matrices must have the same size");
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            arr[i][j] += rhs[i][j];
        }
    }

    return *this;
}

Matrix operator-(Matrix lhs, const Matrix& rhs) {
    lhs -= rhs;
    return lhs;
}

Matrix& Matrix::operator*=(const complex& rhs) {
    for (column_type& row : arr) {
        for (complex& element : row) {
            element *= rhs;
        }
    }

    return *this;
}

Matrix operator*(Matrix lhs, const complex& rhs) {
    lhs *= rhs;
    return lhs;
}

Matrix& Matrix::operator/=(const complex& rhs) {
    for (column_type& row : arr) {
        for (complex& element : row) {
            element /= rhs;
        }
    }

    return *this;
}

Matrix operator/(Matrix lhs, const complex& rhs) {
    lhs /= rhs;
    return lhs;
}

Matrix& Matrix::operator*=(const Matrix& rhs) {
    // MxN matrix * NxP matrix = MxP matrix
    if (columns != rhs.rows) {
        throw errors::malformed_expression("matrices are not compatible sizes to be multiplied");
    }

    columns = rhs.columns;
    std::vector<column_type> temp_arr = std::move(arr);

    arr.reserve(rows);
    for (int i = 0; i < rows; ++i) {
        arr.push_back(column_type(columns));
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rhs.columns; ++j) {
            arr[i][j] = 0;
            for (int k = 0; k < rhs.rows; ++k) {
                arr[i][j] += temp_arr[i][k] * rhs[k][j];
            }
        }
    }

    return *this;
}

Matrix operator*(Matrix lhs, const Matrix& rhs) {
    lhs *= rhs;
    return lhs;
}

Matrix Matrix::minor(size_t row, size_t column) const {
    Matrix ret{rows - 1, columns - 1, context};

    size_t ret_i = 0, ret_j;

    for (size_t i = 0; i < rows; ++i) {
        ret_j = 0;
        if (i != row) {
            for (size_t j = 0; j < columns; ++j) {
                if (j != column) {
                    ret[ret_i][ret_j] = arr[i][j];
                    ++ret_j;
                }
            }
            ++ret_i;
        }
    }
    return ret;
}

std::shared_ptr<Matrix> Matrix::transpose() const {
    if (cache.transpose) {
        return cache.transpose;
    }

    auto ret = std::make_unique<Matrix>(columns, rows, context);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            (*ret)[j][i] = arr[i][j];
        }
    }

    return ret;
}

std::shared_ptr<Matrix> Matrix::rref() const {
    if (cache.rref) {
        return cache.rref;
    }

    auto rref = std::make_shared<Matrix>(rows, columns, context);

    cache.rref = rref;
    return rref;
}

std::string Matrix::to_string() const {
    std::string ret;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            ret += ::to_string(arr[i][j]);
            if (j != columns - 1) {
                ret += ' ';
            }
        }
        ret += '\n';
    }

    return ret;
}

void Matrix::reset(std::string name, Variable value)  {
    context->reset(name, value);
    calculator.reset(name);
}

void Matrix::resize(size_t rows, size_t columns) {
    std::vector<column_type> temp_arr = arr;
    *this = Matrix{rows, columns, context};

    for (size_t i = 0; i < temp_arr.size() && i < rows; ++i) {
        for (size_t j = 0; j < temp_arr[0].size() & j < columns; ++j) {
            arr[i][j] = temp_arr[i][j];
        }
    }
}

Matrix Matrix::cofactor() const {
    Matrix ret{rows, columns, context};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            ret[i][j] = sign(i, j) * minor(i, j).determinant();
        }
    }
    
    return ret;
}

Matrix Matrix::adjoint() const {
    return *(cofactor().transpose());
}
