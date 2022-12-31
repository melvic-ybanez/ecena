//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef ECENA_TUPLES_H
#define ECENA_TUPLES_H

#include <vector>
#include "math_utils.h"

#define TRANSFORM(matrix) matrix * (*this)

namespace rt::math {
    class Tuple {
    protected:
        std::vector<real> elems;
    public:
        Tuple();

        Tuple(real x, real y, real z, real w);

        real x() const;

        real y() const;

        real z() const;

        real w() const;

        Tuple operator-() const;

        Tuple operator*(real scalar) const;

        Tuple operator/(real scalar) const;

        bool operator==(const Tuple &other) const;

        bool operator!=(const Tuple &other) const;

        Tuple translate(real x, real y, real z) const;

        Tuple scale(real x, real y, real z) const;

        Tuple rotate_x(real r) const;

        Tuple rotate_y(real r) const;

        Tuple rotate_z(real r) const;

        Tuple shear(real xy, real xz, real yx, real yz, real zx, real zy) const;

        friend std::ostream &operator<<(std::ostream &out, const Tuple &t);
    };

    Tuple operator+(const Tuple &u, const Tuple &v);

    Tuple operator-(const Tuple &u, const Tuple & v);

    class Point : public Tuple {
    public:
        Point(real x, real y, real z);
        Point();
        explicit Point(const Tuple &tuple);
    };

    class Vec : public Tuple {
    public:
        Vec(real x, real y, real z);
        Vec();
        explicit Vec(const Tuple &tuple);

        real magnitude() const;

        /**
         * Converts this vector into a unit vector.
         */
        Vec normalize() const;

        real dot(const Vec &other) const;

        real dot(const Tuple &other) const;

        Vec cross(const Vec &other) const;

        Vec reflect(const Vec &normal) const;
    };

    class Color : public Tuple {
    public:
        Color(real r, real g, real b);
        Color(const Tuple &tuple);
        Color();

        static const Color black_;

        static const Color red_;

        static const Color white_;

        real red() const;
        real green() const;
        real blue() const;

        void write_to(std::ofstream &out) const;
    };

    Color operator*(const Color &c1, const Color &c2);
}

namespace rt {
    using Tuple = math::Tuple;
    using Point = math::Point;
    using Vec = math::Vec;
    using Color = math::Color;
}

#endif //ECENA_TUPLES_H