//
// Created by Melvic Ybanez on 12/17/22.
//

#include <array>
#include <iostream>
#include "../include/ray.h"

namespace rt {
    Ray::Ray(const Point &origin, const Vec &direction): origin{origin}, direction{direction} {}

    Ray::Ray(Point &&origin, Vec &&direction): origin{std::move(origin)}, direction{std::move(direction)} {}

    Ray::Ray(Tuple &&origin, Tuple &&direction) : Ray(Point{origin}, Vec{direction}) {}

    Point Ray::at(real t) const {
        return rt::Point{origin + direction * t};
    }

    std::ostream &operator<<(std::ostream &out, const Ray &ray) {
        return out << "origin: " << ray.origin << "; direction " << ray.direction;
    }

    Ray Ray::transform(const math::Matrix<4, 4> &matrix) const {
        return Ray{matrix * origin, matrix * direction};
    }

    Ray Ray::translate(real x, real y, real z) const {
        return transform(math::matrix::translation(x, y, z));
    }

    Ray Ray::scale(real x, real y, real z) const {
        return transform(math::matrix::scaling(x, y, z));
    }

    Ray Ray::rotate_x(real r) const {
        return transform(math::matrix::rotation_x(r));
    }

    Ray Ray::rotate_y(real r) const {
        return transform(math::matrix::rotation_y(r));
    }

    Ray Ray::rotate_z(real r) const {
        return transform(math::matrix::rotation_z(r));
    }

    Ray Ray::shear(real xy, real xz, real yx, real yz, real zx, real zy) const {
        return transform(math::matrix::shearing(xy, xz, yx, yz, zx, zy));
    }

    bool Ray::operator==(const Ray &that) const {
        return this->origin == that.origin && this->direction == that.direction;
    }
}
