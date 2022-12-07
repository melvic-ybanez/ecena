//
// Created by Melvic Ybanez on 12/8/22.
//

#include "../include/tuple.h"

namespace rt {
    Tuple::Tuple() : elems{0, 0, 0, 0} {}

    Tuple::Tuple(real x, real y, real z, real w) : elems{x, y, z, w} {}

    real Tuple::x() const {
        return elems[0];
    }

    real Tuple::y() const {
        return elems[1];
    }

    real Tuple::z() const {
        return elems[2];
    }

    real Tuple::w() const {
        return elems[3];
    }

    Tuple Tuple::operator-() const {
        return {-x(), -y(), -z(), -w()};
    }

    bool Tuple::operator==(const Tuple &other) {
        for (int i = 0; i < 4; i++) {
            if (elems[i] != other.elems[i]) return false;
        }
        return true;
    }

    std::ostream &operator<<(std::ostream &out, const Tuple &t) {
        return out << t.x() << ' ' << t.y() << ' ' << t.z() << ' ' << t.w();
    }

    Tuple operator+(const Tuple &u, const Tuple &v) {
        return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z(), u.w() + v.w()};
    }

    Tuple operator-(const Tuple &u, const Tuple & v) {
        return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z(), u.w() - v.w()};
    }

    Point::Point(): Tuple(0, 0, 0, 1) {}

    Point::Point(real x, real y, real z) : Tuple(x, y, z, 1) {}

    Vec::Vec(): Tuple(0, 0, 0, 0) {}

    Vec::Vec(real x, real y, real z) : Tuple(x, y, z, 0) {}
}