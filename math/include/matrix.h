//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include "utils.h"

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

        Matrix<R, C> operator*(const Matrix<R, C> &other) const {
            Matrix<R, C> product{{}};
            for (int r = 0; r < R; r++) {
                for (int c = 0; c < C; c++) {
                    real sum = 0;
                    for (int j = 0; j < other.elems_.size(); j++) {
                        sum += elems_[r][j] * other.elems_[j][c];
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
}

#endif //ECENA_MATRIX_H