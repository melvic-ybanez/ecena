//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>

template<typename T>
class vec3 {
    double elems[3];
public:
    vec3() : elems{0, 0, 0} {}
    vec3(double x, double y, double z) : elems{x, y, z} {}

    double x() const {
        return elems[0];
    }

    double y() const {
        return elems[1];
    }

    double z() const {
        return elems[2];
    }

    vec3<T> operator-() const {
        return { -x(), -y(), -z() };
    }

    double operator[](int i) const {
        return elems[i];
    }

    double& operator[](int i) {
        return elems[i];
    }

    vec3<T>& operator+=(const vec3<T> &v) {
        elems[0] += v.x();
        elems[1] += v.y();
        elems[2] += v.z();
        return *this;
    }

    vec3<T>& operator*=(const double t) {
        elems[0] *= t;
        elems[1] *= t;
        elems[2] *= t;
        return *this;
    }

    vec3<T>& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return x() * x() + y() * y() + z() * z();
    }

    vec3<T> unit() {
        return this / length();
    }
};

struct Point {};
struct Color {};

using point3 = vec3<Point>;
using color = vec3<Color>;

template<typename T>
std::ostream& operator<<(std::ostream &out, const vec3<T> &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

template<typename T>
vec3<T> binop(const vec3<T> &u, const vec3<T> &v, std::function<double (double, double)> f) {
    return { f(u.x(), v.x()), f(u.y(), v.y()), f(u.z(), v.z()) };
}

template<typename T>
vec3<T> operator+(const vec3<T> &u, const vec3<T> &v) {
    return binop(u, v, [](double a, double b) { return a + b; });
}

template<typename T>
vec3<T> operator*(const vec3<T> &u, const vec3<T> &v) {
    return binop(u, v, [](double a, double b) { return a * b; });
}

template<typename T>
vec3<T> operator*(double t, const vec3<T> &v) {
    return { t * v.x(), t * v.y(), t * v.z() };
}

template<typename T>
vec3<T> operator*(const vec3<T> &v, double t) {
    return t * v;
}

template<typename T>
vec3<T> operator/(vec3<T> &v, double t) {
    return 1 / t * v;
}

template<typename T>
double dot(const vec3<T> &u, const vec3<T> &v) {
    return u.x() * v.x() + u.y * v.y() + u.z() * v.z();
}

template<typename T>
vec3<T> cross(const vec3<T> &u, const vec3<T> &v) {
    return {
        u.y() * v.z() - u.z() * v.y(),
        u.z() * v.x() - u.x() * v.z(),
        u.x() * v.y() - u.y() * v.x()
    };
}

#endif //RAYTRACER_VEC3_H
