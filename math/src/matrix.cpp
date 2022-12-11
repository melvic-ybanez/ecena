//
// Created by Melvic Ybanez on 12/11/22.
//

#include "../include/matrix.h"

namespace rt::math {
    Matrix::Matrix(MatrixTable &elems) : elems{elems} {}

    MatrixRow &Matrix::operator[](int row) {
        return elems[row];
    }
}