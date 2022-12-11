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
        MatrixTable &elems_;

    public:
        explicit Matrix(MatrixTable &elems);

        MatrixRow &operator[](int row);

        bool operator==(const Matrix &other) const;

        bool operator!=(const Matrix &other) const;

        const MatrixTable &elems() const;
    };

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
}

#endif //ECENA_MATRIX_H