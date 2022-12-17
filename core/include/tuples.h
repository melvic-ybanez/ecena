//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef ECENA_TUPLES_H
#define ECENA_TUPLES_H

#include <cmath>
#include <iostream>
#include <vector>
#include "../../math/include/utils.h"

#define TRANSFORM(matrix) matrix * (*this)

namespace rt {
    class Tuple {
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
    };

    std::ostream &operator<<(std::ostream &out, const Tuple &t);

    Tuple operator+(const Tuple &u, const Tuple &v);

    Tuple operator-(const Tuple &u, const Tuple & v);

    class Point : public Tuple {
    public:
        Point(real x, real y, real z);
        Point();
    };

    class Vec : public Tuple {
    public:
        Vec(real x, real y, real z);
        Vec();

        real magnitude() const;

        Vec normalize() const;

        real dot(const Vec &other) const;

        Vec cross(const Vec &other) const;
    };

    class Color : public Tuple {
    public:
        Color(real r, real g, real b);
        Color();

        static const Color black_;

        static const Color red_;

        real red() const;
        real green() const;
        real blue() const;

        void write_to(std::ofstream &out) const;
    };

    Color operator*(const Color &c1, const Color &c2);
}

#endif //ECENA_TUPLES_H
