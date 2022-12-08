//
// Created by Melvic Ybanez on 12/8/22.
//

#include <iomanip>
#include <fstream>
#include "../include/tuples.h"

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

    Tuple Tuple::operator*(real scalar) const {
        return {scalar * x(), scalar * y(), scalar * z(), scalar * w()};
    }

    Tuple Tuple::operator/(real scalar) const {
        return *this * (1 / scalar);
    }

    bool Tuple::operator==(const Tuple &other) {
        for (int i = 0; i < 4; i++) {
            if (!rt::compare_reals(elems[i], other.elems[i])) return false;
        }
        return true;
    }

    std::ostream &operator<<(std::ostream &out, const Tuple &t) {
        return out << t.x() << ' ' << t.y() << ' ' << t.z() << ' ' << t.w();
    }

    Tuple operator+(const Tuple &u, const Tuple &v) {
        return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z(), u.w() + v.w()};
    }

    Tuple operator-(const Tuple &u, const Tuple &v) {
        return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z(), u.w() - v.w()};
    }

    Point::Point() : Tuple(0, 0, 0, 1) {}

    Point::Point(real x, real y, real z) : Tuple(x, y, z, 1) {}

    Vec::Vec() : Tuple(0, 0, 0, 0) {}

    Vec::Vec(real x, real y, real z) : Tuple(x, y, z, 0) {}

    real Vec::magnitude() const {
        return std::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
    }

    Vec Vec::normalize() const {
        auto m = magnitude();
        return {x() / m, y() / m, z() / m};
    }

    real Vec::dot(const Vec &other) const {
        return x() * other.x() + y() * other.y() + z() * other.z() + w() * other.w();
    }

    Vec Vec::cross(const Vec &other) const {
        return {
                y() * other.z() - z() * other.y(),
                z() * other.x() - x() * other.z(),
                x() * other.y() - y() * other.x()
        };
    }

    Color::Color() : Tuple(0, 0, 0, 0) {}

    Color::Color(real r, real g, real b) : Tuple(r, g, b, 0) {}

    real Color::red() const {
        return x();
    }

    real Color::green() const {
        return y();
    }

    real Color::blue() const {
        return z();
    }

    Color operator*(const Color &c1, const Color &c2) {
        return {
                c1.red() * c2.red(),
                c1.green() * c2.green(),
                c1.blue() * c2.blue()
        };
    }

    void Color::write_to(std::ofstream &out) const {
        auto r = static_cast<int>(255.999 * red());
        auto g = static_cast<int>(255.999 * green());
        auto b = static_cast<int>(255.999 * blue());

        out << r << ' ' << g << ' ' << b << std::endl;
    }
}