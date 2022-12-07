//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_TUPLE_H
#define RAYTRACER_TUPLE_H

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
    };
}

#endif //RAYTRACER_TUPLE_H
