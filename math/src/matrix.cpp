//
// Created by Melvic Ybanez on 12/14/22.
//

#include <array>
#include "../include/matrix.h"

namespace rt::math::matrix {
    /**
     * For any 2x2 matrix of the following form:
     * [a b]
     * [c d]
     * determinants = ad - bc
     */
    template<>
    real Matrix<2, 2>::determinant() const {
        auto a = (*this)[0][0];
        auto b = (*this)[0][1];
        auto c = (*this)[1][0];
        auto d = (*this)[1][1];
        return a * d - b * c;
    }

    Matrix<4, 4> translation(real x, real y, real z) {
        return Matrix<4, 4>{
                {{{1, 0, 0, x},
                  {0, 1, 0, y},
                  {0, 0, 1, z},
                  {0, 0, 0, 1}}}
        };
    }
}