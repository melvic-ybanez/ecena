//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include "utils.h"
#include "../../core/include/tuples.h"

namespace rt::math {
    template<int S>
    using MatrixRow = std::array<real, S>;

    template<int R, int C>
    using MatrixTable = std::array<MatrixRow<C>, R>;

    template<int R, int C>
    class Matrix {
        MatrixTable<R, C> elems_;

    public:
        explicit Matrix(MatrixTable<R, C> elems) : elems_{std::move(elems)} {}

        MatrixRow<C> &operator[](int row) {
            return elems_[row];
        }

        bool operator==(const Matrix<R, C> &other) const {
            bool equals = true;
            for (int r = 0; r < R; r++) {
                for (int c = 0; c < C; c++) {
                    if (!compare_reals(this->elems_[r][c], other.elems_[r][c])) {
                        equals = false;
                        break;
                    }
                }
                if (!equals) break;
            }
            return equals;
        }

        template<int R1, int C1>
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
        template<int C0>
        Matrix<R, C0> operator*(const Matrix<C, C0> &other) const {
            Matrix<R, C0> product{{}};
            for (int r = 0; r < R; r++) {
                for (int c = 0; c < C0; c++) {
                    real sum = 0;
                    for (int j = 0; j < C; j++) {
                        sum += elems()[r][j] * other.elems()[j][c];
                    }
                    product[r][c] = sum;
                }
            }
            return product;
        }
    };

    template<int R, int C>
    std::ostream &operator<<(std::ostream &out, const Matrix<R, C> &matrix) {
        for (const auto &row: matrix.elems()) {
            for (const auto &elem: row) {
                out << elem << ' ';
            }
            out << std::endl;
        }
        return out;
    }

    template<int R>
    Tuple operator*(const Matrix<R, 4> &matrix, const Tuple &tuple) {
        Matrix<4, 1> other{{{{tuple.x()}, {tuple.y()}, {tuple.z()}, {tuple.w()}}}};
        Matrix<R, 1> result = matrix * other;
        return Tuple{result[0][0], result[0][1], result[0][2], result[0][3]};
    }
}

#endif //ECENA_MATRIX_H