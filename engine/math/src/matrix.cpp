//
// Created by Melvic Ybanez on 12/14/22.
//

#include <array>
#include <cmath>
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

    Matrix<4, 4> scaling(real x, real y, real z) {
        return Matrix<4, 4>{
                {{{x, 0, 0, 0},
                  {0, y, 0, 0},
                  {0, 0, z, 0},
                  {0, 0, 0, 1}}}
        };
    }

    Matrix<4, 4> rotation_x(real r) {
        return Matrix<4, 4>{
                {{{1, 0, 0, 0},
                  {0, std::cos(r), -std::sin(r), 0},
                  {0, std::sin(r), std::cos(r), 0},
                  {0, 0, 0, 1}}}
        };
    }

    Matrix<4, 4> rotation_y(real r) {
        return Matrix<4, 4>{
                {{{std::cos(r), 0, std::sin(r), 0},
                  {0, 1, 0, 0},
                  {-std::sin(r), 0, std::cos(r), 0},
                  {0, 0, 0, 1}}}
        };
    }

    Matrix<4, 4> rotation_z(real r) {
        return Matrix<4, 4>{
                {{{std::cos(r), -std::sin(r), 0, 0},
                  {std::sin(r), std::cos(r), 0, 0},
                  {0, 0, 1, 0},
                  {0, 0, 0, 1}}}
        };
    }

    Matrix<4, 4> shearing(real xy, real xz, real yx, real yz, real zx, real zy) {
        return Matrix<4, 4>{
                {{{1, xy, xz, 0},
                  {yx, 1, yz, 0},
                  {zx, zy, 1, 0},
                  {0, 0, 0, 1}}}
        };
    }

    Matrix<4, 4> view_transform(const Point &from, const Point &to, const Vec &up) {
        auto forward = Vec(to - from).normalize();
        auto left = forward.cross(up.normalize());
        auto true_up = left.cross(forward);
        Matrix<4, 4> orientation{
                {{{left.x(), left.y(), left.z(), 0},
                  {true_up.x(), true_up.y(), true_up.z(), 0},
                  {-forward.x(), -forward.y(), -forward.z(), 0},
                  {0, 0, 0, 1}}}
        };
        return orientation * translation(-from.x(), -from.y(), -from.z());
    }
}