//
// Created by Melvic Ybanez on 1/6/23.
//

#ifndef ECENA_TRANSFORM_H
#define ECENA_TRANSFORM_H

#include "math_utils.h"
#include "matrix.h"

namespace rt::math {
    template<typename T>
    T &transform(T &obj, Matrix<4, 4> transformation) {
        obj.transformation = transformation * obj.transformation;
        return obj;
    }

    template<typename T>
    T &translate(T &obj, real x, real y, real z) {
        return transform(obj, math::matrix::translation(x, y, z));
    }

    template<typename T>
    T &scale(T &obj, real x, real y, real z) {
        return transform(obj, math::matrix::scaling(x, y, z));
    }

    template<typename T>
    T &rotate_x(T &obj, real r) {
        return transform(obj, math::matrix::rotation_x(r));
    }

    template<typename T>
    T &rotate_y(T &obj, real r) {
        return transform(obj, math::matrix::rotation_y(r));
    }

    template<typename T>
    T &rotate_z(T &obj, real r) {
        return transform(obj, math::matrix::rotation_z(r));
    }

    template<typename T>
    T &shear(T &obj, real xy, real xz, real yx, real yz, real zx, real zy) {
        return transform(obj, math::matrix::shearing(xy, xz, yx, yz, zx, zy));
    }
}

#endif //ECENA_TRANSFORM_H
