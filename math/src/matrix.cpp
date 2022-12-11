//
// Created by Melvic Ybanez on 12/11/22.
//

#include <iostream>
#include <ranges>
#include "../include/matrix.h"

namespace rt::math {
    Matrix::Matrix(MatrixTable &elems) : elems_{elems} {}

    MatrixRow &Matrix::operator[](int row) {
        return elems_[row];
    }

    bool Matrix::operator==(const Matrix &other) const {
        bool equals = true;
        for (int r = 0; r < this->elems_.size(); r++) {
            for (int c = 0; c < this->elems_[0].size(); c++) {
                if (!compare_reals(this->elems_[r][c], other.elems_[r][c])) {
                    equals = false;
                    break;
                }
            }
            if (!equals) break;
        }
        return equals;
    }

    bool Matrix::operator!=(const Matrix &other) const {
        return !(*this == other);
    }

    const MatrixTable &Matrix::elems() const {
        return elems_;
    }

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
        for (const auto &row: matrix.elems()) {
            for (const auto &elem: row) {
                out << elem << ' ';
            }
            out << std::endl;
        }
        return out;
    }
}