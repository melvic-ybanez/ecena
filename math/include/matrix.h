//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include <vector>
#include "utils.h"

namespace rt::math {
    template<int S>
    using MatrixRow = std::array<real, S>;

    template<int R, int C>
    using MatrixTable = std::array<MatrixRow<C>, R>;

    template<int R, int C>
    class Matrix {
        MatrixTable<R, C> &elems_;

    public:
        explicit Matrix(MatrixTable<R, C> &elems) : elems_{elems} {}

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

    private:
        template<int R0, int C0>
        static Matrix<R0, C0> init_zero() {
            MatrixTable<R0, C0> table;
            for (int r = 0; r < R0; r++) {
                MatrixRow<C> row;
                for (int c = 0; c < C0; c++) {
                    row.push_back(0);
                }
                table.push_back(row);
            }
            return Matrix{table};
        }

    public:
        template<int R0, int C0>
        static Matrix<R0, C0> zero{init_zero<R0, C0>()};
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