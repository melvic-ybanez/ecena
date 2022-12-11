//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include <vector>
#include "utils.h"

namespace rt::math {
    using MatrixRow = std::vector<real>;
    using MatrixTable = std::vector<MatrixRow>;

    class Matrix {
        MatrixTable &elems;

    public:
        explicit Matrix(MatrixTable &elems);

        MatrixRow &operator[](int row);
    };
}

#endif //ECENA_MATRIX_H