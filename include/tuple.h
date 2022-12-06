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

        Tuple<W> operator-() const {
            return {-x(), -y(), -z(), W};
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

        Tuple<W> &operator+=(const Tuple<W> &v) {
            elems[0] += v.x();
            elems[1] += v.y();
            elems[2] += v.z();
            elems[3] += v.type();
            return *this;
        }

        Tuple<W> &operator*=(const real t) {
            elems[0] *= t;
            elems[1] *= t;
            elems[2] *= t;
            elems[3] *= t;
            return *this;
        }

        Tuple<W> &operator/=(const real t) {
            return *this *= 1 / t;
        }

        real length() const {
            return sqrt(length_squared());
        }

        real length_squared() const {
            return x() * x() + y() * y() + z() * z();
        }

        Tuple<W> unit() {
            return this / length();
        }
    };

    using Point = Tuple<1>;
    using Color = Tuple<0>;
    using Vec = Tuple<0>;

    Point *point(real x, real y, real z) {
        return new Tuple<1>(x, y, z);
    }

    Vec *vec(real x, real y, real z) {
        return new Tuple<0>(x, y, z);
    }

    template<int w>
    std::ostream &operator<<(std::ostream &out, const Tuple<w> &v) {
        return out << v.x() << ' ' << v.y() << ' ' << v.z();
    }

    template<int w0, int w1>
    Tuple<w0> binop(const Tuple<w0> &u, const Tuple<w1> &v, std::function<double(double, double)> &&f) {
        return {f(u.x(), v.x()), f(u.y(), v.y()), f(u.z(), v.z())};
    }

    template<int w0, int w1>
    Tuple<w0> operator+(const Tuple<w0> &u, const Tuple<w1> &v) {
        return binop(u, v, [](real a, real b) { return a + b; });
    }

    template<int w>
    Tuple<w> operator*(const Tuple<w> &u, const Tuple<w> &v) {
        return binop(u, v, [](real a, real b) { return a * b; });
    }

    template<int w>
    Tuple<w> operator*(real t, const Tuple<w> &v) {
        return {t * v.x(), t * v.y(), t * v.z()};
    }

    template<int w>
    Tuple<w> operator*(const Tuple<w> &v, real t) {
        return t * v;
    }

    template<int w>
    Tuple<w> operator/(Tuple<w> &v, real t) {
        return 1 / t * v;
    }

    template<int w>
    real dot(const Tuple<w> &u, const Tuple<w> &v) {
        return u.x() * v.x() + u.y * v.y() + u.z() * v.z();
    }
}

#endif //RAYTRACER_TUPLE_H
