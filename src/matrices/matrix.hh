#ifndef MATRIX_HH
#define MATRIX_HH

#include <array>
#include <assert.h>
#include <complex>
#include <string>

using std::array, std::string;
typedef std::complex<double> complex;
typedef std::size_t size_t;

template <size_t rows, size_t columns>
class Matrix {
public:
    array<array<complex, columns>, rows> data;

    Matrix();
    Matrix(array<array<complex, columns>, rows>& data);

    inline array<complex, columns>& operator[](size_t idx) { return data[idx]; }
    const inline array<complex, columns>& operator[](size_t idx) const { return data[idx]; }

    inline Matrix& operator+=(const Matrix& rhs) {
        for (const auto& [this_row, rhs_row] : data, rhs.data) {
            for (auto& [this_element, rhs_element]: this_row, rhs_row) {
                this_element += rhs_element;
            }
        }

        return *this;
    }
    inline friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
        lhs += rhs;
        return lhs;
    }
    inline Matrix& operator-=(const Matrix& rhs) {
        for (const auto& [this_row, rhs_row] : data, rhs.data) {
            for (auto& [this_element, rhs_element]: this_row, rhs_row) {
                this_element -= rhs_element;
            }
        }

        return *this;
    }
    inline friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
        lhs -= rhs;
        return lhs;
    }
    inline Matrix& operator*=(const complex& rhs) {
        for (const auto& row : data) {
            for (auto& element : row) {
                element *= rhs;
            }
        }

        return *this;
    }
    inline friend Matrix operator*(Matrix lhs, const complex& rhs) {
        lhs *= rhs;
        return lhs;
    }
    inline Matrix& operator*=(const Matrix& rhs) {
        assert(columns == rhs.rows);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rhs.columns; j++) {
                int sum = 0;
                for (int k = 0; k < rhs.columns; k ++) {
                    sum += data[i][k] * rhs[k][j];
                }
                data[i][j] = sum;
            }
        }
    }
    inline friend Matrix operator*(Matrix lhs, const Matrix& rhs) {
        lhs *= rhs;
        return lhs;
    }

    inline Matrix& minor(size_t row, size_t column) const {
        Matrix<rows - 1, columns - 1> ret;

        size_t ret_i = 0, ret_j;

        for (size_t i = 0; i < rows; i++) {
            ret_j = 0;
            if (i != row) {
                for (size_t j = 0; j < columns; j++) {
                    if (j != column) {
                        ret[ret_i][ret_j] = data[i][j];
                        ret_j++;
                    }
                }
                ret_i++;
            }
        }
        return 0;
    }
    inline Matrix& transpose() const {
        Matrix<columns, rows> ret;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                ret[i][j] = data[i][j];
            }
        }
    }

    inline string to_string() const {
        string ret;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                ret += std::to_string(data[i][j]);
                if (j != columns - 1) {
                    ret += ' ';
                }
            }
            ret += '\n';
        }

        return ret;
    }
};

inline int sign(size_t i, size_t j) {
    if ((i + j) % 2) {
        return -1;
    }

    return 1;
}

template <size_t rows, size_t columns>
class SquareMatrix : public Matrix<rows, columns>  {
    inline SquareMatrix& cofactor() const {
        SquareMatrix<rows - 1, columns - 1> minor_m;
        SquareMatrix<rows, columns> ret;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                ret[i][j] = sign(i, j) * this->minor(i, j).determinant();
            }
        }
    }
    inline SquareMatrix& adjugate() const {
        return this->cofactor().transpose();
    }
    inline complex determinant() const {
        if (rows == 1) {
            return (*this)[0][0];
        } else if (rows == 2) {
            return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
        }

        complex ret = 0;
        Matrix<rows - 1, columns - 1> res;

        for (int i = 0; i < columns; i++) {
            ret += (*this)[i][0] * sign(i, 0) * this->minor(i, 0).determinant();
        }

        return ret;
    }
    inline SquareMatrix& inverse() const {
        return adjugate() / determinant();
    }
    inline complex trace() {
        complex ret = 0;

        for (int i = 0; i < rows; i++) {
            ret += (*this)[i][i];
        }

        return ret;
    }
};

#endif
