//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include "utils.h"
#include "../../core/include/tuples.h"

namespace rt::math::matrix {
    template<size_t S>
    using MatrixRow = std::array<real, S>;

    template<size_t R, size_t C>
    using MatrixTable = std::array<MatrixRow<C>, R>;

    template<size_t R, size_t C>
    class Matrix {
        MatrixTable<R, C> elems_;

    public:
        explicit Matrix(MatrixTable<R, C> elems) : elems_{std::move(elems)} {}

        Matrix(const Matrix<R, C> &from) : elems_{from.elems_} {}

        explicit Matrix() : elems_({}) {}

        MatrixRow<C> &operator[](size_t row) {
            return elems_[row];
        }

        const MatrixRow<C> &operator[](size_t row) const {
            return elems_[row];
        }

        bool operator==(const Matrix<R, C> &other) const {
            bool equals = true;
            for (auto r = 0; r < R; r++) {
                for (auto c = 0; c < C; c++) {
                    if (!compare_reals(this->elems_[r][c], other.elems_[r][c])) {
                        equals = false;
                        break;
                    }
                }
                if (!equals) break;
            }
            return equals;
        }

        template<size_t R1, size_t C1>
        bool operator!=(const Matrix<R1, C1> &other) const {
            return !(*this == other);
        }

        const MatrixTable<R, C> &elems() const {
            return elems_;
        }

        /**
         * Multiply this matrix by another matrix, provided that the former's number of columns
         * is the same as the latter's number of rows.
         */
        template<size_t C0>
        Matrix<R, C0> operator*(const Matrix<C, C0> &other) const {
            Matrix<R, C0> product;
            for (auto r = 0; r < R; r++) {
                for (auto c = 0; c < C0; c++) {
                    real sum = 0;
                    for (auto j = 0; j < C; j++) {
                        sum += elems()[r][j] * other.elems()[j][c];
                    }
                    product[r][c] = sum;
                }
            }
            return product;
        }

        Matrix<R - 1, C - 1> submatrix(size_t row, size_t col) const {
            Matrix<R - 1, C - 1> result;
            auto r0 = 0;
            for (auto r = 0; r < R; r++) {
                if (r == row) continue;
                auto c0 = 0;
                for (auto c = 0; c < C; c++) {
                    if (c == col) continue;
                    result[r0][c0] = (*this)[r][c];
                    c0++;
                }
                r0++;
            }
            return result;
        }

        real determinant() const {
            auto determinant = 0;
            for (auto c = 0; c < C; c++) {
                determinant += (*this)[0][c] * cofactor(0, c);
            }
            return determinant;
        }

        /**
         * Computes the determinants of the submatrix
         */
        real minor(size_t row, size_t col) const {
            return submatrix(row, col).determinant();
        }

        /**
         * A cofactor is a minor that has its sign possibly flipped according to the
         * following table
         * [+ - +]
         * [- + -]
         * [+ - +]
         * If the minor lands on a positive spot, it gets to keep its value. Otherwise, its
         * sign is negated.
         */
        real cofactor(size_t row, size_t col) const {
            auto m = minor(row, col);
            if ((row + col) % 2 != 0) return -m;
            return m;
        }
    };  // end of Matrix

    template<size_t R, size_t C>
    std::ostream &operator<<(std::ostream &out, const Matrix<R, C> &matrix) {
        for (const auto &row: matrix.elems()) {
            for (const auto &elem: row) {
                out << elem << ' ';
            }
            out << std::endl;
        }
        return out;
    }

    template<size_t R>
    Tuple operator*(const Matrix<R, 4> &matrix, const Tuple &tuple) {
        Matrix<4, 1> other{{{{tuple.x()}, {tuple.y()}, {tuple.z()}, {tuple.w()}}}};
        Matrix<R, 1> result = matrix * other;
        return Tuple{result[0][0], result[0][1], result[0][2], result[0][3]};
    }

    namespace {
        template<size_t R, size_t C>
        Matrix<R, C> identity_value{{}};
    }

    template<size_t R, size_t C>
    Matrix<R, C> identity() {
        // If it has a 1, then it's already initialized. Just return it immediately.
        if (identity_value<R, C>[0][0] == 1) return identity_value<R, C>;

        MatrixTable<R, C> table{};

        // set all the elements along the diagonal to 1
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C; c++) {
                if (r == c) table[r][c] = 1;
            }
        }

        identity_value<R, C> = Matrix{table};
        return identity_value<R, C>;
    }

    template<size_t S>
    Matrix<S, S> transpose(const Matrix<S, S> &matrix) {
        Matrix<S, S> result;
        for (auto r = 0; r < S; r++) {
            for (auto c = 0; c < S; c++) {
                result[r][c] = matrix[c][r];
            }
        }
        return result;
    }

    /**
     * Computes the determinants of any 2x2 matrix.
     */
    template<>
    real Matrix<2, 2>::determinant() const;
}

namespace rt::math {
    template<size_t R, size_t C>
    using Matrix = rt::math::matrix::Matrix<R, C>;
}

#endif //ECENA_MATRIX_H