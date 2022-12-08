//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_TUPLES_H
#define RAYTRACER_TUPLES_H

#include <cmath>
#include <iostream>
#include "../include/rtmath.h"

namespace rt {
    class Tuple {
        double elems[4];
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

        bool operator==(const Tuple &other);
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

        real red() const;
        real green() const;
        real blue() const;
    };
}

#endif //RAYTRACER_TUPLES_H
