//
// Created by Melvic Ybanez on 12/8/22.
//

#include <array>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "../include/tuples.h"
#include "../include/matrix.h"

namespace rt::math {
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

    bool Tuple::operator==(const Tuple &other) const {
        for (int i = 0; i < elems.size(); i++) {
            if (!math::compare_reals(elems[i], other.elems[i])) return false;
        }
        return true;
    }

    bool Tuple::operator!=(const Tuple &other) const {
        return !(*this == other);
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

    Tuple Tuple::translate(real x, real y, real z) const {
        return TRANSFORM(math::matrix::translation(x, y, z));
    }

    Tuple Tuple::scale(real x, real y, real z) const {
        return TRANSFORM(math::matrix::scaling(x, y, z));
    }

    Tuple Tuple::rotate_x(real r) const {
        return TRANSFORM(math::matrix::rotation_x(r));
    }

    Tuple Tuple::rotate_y(real r) const {
        return TRANSFORM(math::matrix::rotation_y(r));
    }

    Tuple Tuple::rotate_z(real r) const {
        return TRANSFORM(math::matrix::rotation_z(r));
    }

    Tuple Tuple::shear(real xy, real xz, real yx, real yz, real zx, real zy) const {
        return TRANSFORM(math::matrix::shearing(xy, xz, yx, yz, zx, zy));
    }

    Point::Point() : Tuple(0, 0, 0, 1) {}

    Point::Point(real x, real y, real z) : Tuple(x, y, z, 1) {}

    Point::Point(const Tuple &tuple) : Point(tuple.x(), tuple.y(), tuple.z()) {}

    Vec::Vec() : Tuple(0, 0, 0, 0) {}

    Vec::Vec(real x, real y, real z) : Tuple(x, y, z, 0) {}

    Vec::Vec(const Tuple &tuple) : Vec(tuple.x(), tuple.y(), tuple.z()) {}

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

    real Vec::dot(const Tuple &other) const {
        return dot(Vec{other});
    }

    Vec Vec::cross(const Vec &other) const {
        return {
                y() * other.z() - z() * other.y(),
                z() * other.x() - x() * other.z(),
                x() * other.y() - y() * other.x()
        };
    }

    Vec Vec::reflect(const Vec &normal) const {
        return Vec{*this - normal * 2 * this->dot(normal)};
    }

    Color::Color() : Tuple(0, 0, 0, 0) {}

    Color::Color(const Tuple &tuple) : Color(tuple.x(), tuple.y(), tuple.z()) {}

    Color::Color(real r, real g, real b) : Tuple(r, g, b, 0) {}

    const Color Color::black_{0, 0, 0};

    const Color Color::red_{1, 0, 0};

    const Color Color::white_{1, 1, 1};

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