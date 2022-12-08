//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_TUPLES_H
#define RAYTRACER_TUPLES_H

#include <cmath>
#include <iostream>

namespace rt {
    using real = double;

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
    };

    class Color : public Tuple {
    public:
        Color(real x, real y, real z);
        Color();
    };
}

#endif //RAYTRACER_TUPLES_H
