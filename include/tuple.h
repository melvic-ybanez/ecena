//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_TUPLE_H
#define RAYTRACER_TUPLE_H

#include <cmath>
#include <iostream>

namespace rt {
    using real = double;

    template<int W>
    class Tuple {
        double elems[4];
    public:
        Tuple() : elems{0, 0, 0, W} {}

        Tuple(real x, real y, real z) : elems{x, y, z, W} {}

        real x() const {
            return elems[0];
        }

        real y() const {
            return elems[1];
        }

        real z() const {
            return elems[2];
        }

        real w() const {
            return W;
        }

        Tuple<-W> operator-() const {
            return {-x(), -y(), -z()};
        }

        real operator[](int i) const {
            return elems[i];
        }

        real &operator[](int i) {
            return elems[i];
        }

        template<int w1>
        bool operator==(const Tuple<w1> &other) {
            return (elems[0] == other.elems[0])
                   && (elems[1] == other.elems[1])
                   && (elems[2] == other.elems[2])
                   && (elems[3] == other.elems[3]);
        }
    };

    using Point = Tuple<1>;
    using Color = Tuple<0>;
    using Vec = Tuple<0>;

    Point point(real x, real y, real z) {
        return {x, y, z};
    }

    Vec vec(real x, real y, real z) {
        return {x, y, z};
    }

    template<int w>
    std::ostream &operator<<(std::ostream &out, const Tuple<w> &v) {
        return out << v.x() << ' ' << v.y() << ' ' << v.z() << v.w();
    }

    template<int w0, int w1>
    Tuple<w0 + w1> operator+(const Tuple<w0> &u, const Tuple<w1> &v) {
        return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
    }

    template<int w0, int w1>
    Tuple<w0 - w1> operator-(const Tuple<w0> &u, const Tuple<w1> & v) {
        return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
    }
}

#endif //RAYTRACER_TUPLE_H
