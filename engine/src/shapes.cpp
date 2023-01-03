//
// Created by Melvic Ybanez on 12/19/22.
//

#include <cmath>
#include <iostream>
#include "../include/shapes.h"

namespace rt::shapes {
    Shape::Shape() : transformation{math::matrix::identity<4, 4>()} {}

    std::ostream &operator<<(std::ostream &out, const Type &type) {
        switch (type) {
            case Type::sphere:
                return out << "Sphere";
            default:
                return out << "Shape";
        }
    }

    Type Shape::type() const {
        return Type::shape;
    }

    Aggregate Shape::intersect(const Ray &ray) {
        auto local_ray = ray.transform(transformation.inverse());
        return local_intersect(local_ray);
    }

    Vec Shape::normal_at(const Point &point) {
        auto inverse = transformation.inverse();
        auto local_point = inverse * point;
        auto local_normal = local_normal_at(local_point);
        auto world_normal = inverse.transpose() * local_normal;
        return Vec{world_normal}.normalize();
    }

    Aggregate Sphere::local_intersect(const Ray &local_ray) {
        // compute the discriminant
        auto sphere_to_ray = local_ray.origin - Point{0, 0, 0};
        auto a = local_ray.direction.dot(local_ray.direction);
        auto b = 2 * local_ray.direction.dot(sphere_to_ray);
        auto c = Vec{sphere_to_ray}.dot(sphere_to_ray) - 1;
        auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return {};

        auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);

        return {{new Intersection{t1, this}, new Intersection{t2, this}}};
    }

    Type Sphere::type() const {
        return Type::sphere;
    }

    Shape &Shape::translate(real x, real y, real z) {
        transformation = math::matrix::translation(x, y, z);
        return *this;
    }

    Shape &Shape::scale(real x, real y, real z) {
        transformation = math::matrix::scaling(x, y, z);
        return *this;
    }

    Shape &Shape::rotate_x(real r) {
        transformation = math::matrix::rotation_x(r);
        return *this;
    }

    Shape &Shape::rotate_y(real r) {
        transformation = math::matrix::rotation_y(r);
        return *this;
    }

    Shape &Shape::rotate_z(real r) {
        transformation = math::matrix::rotation_z(r);
        return *this;
    }

    Shape &Shape::shear(real xy, real xz, real yx, real yz, real zx, real zy) {
        transformation = math::matrix::shearing(xy, xz, yx, yz, zx, zy);
        return *this;
    }

    Vec Sphere::local_normal_at(const Point &local_point) {
        return local_point - Point{0, 0, 0};
    }

    Type Plane::type() const {
        return Type::plane;
    }

    Vec Plane::local_normal_at(const Point &local_point) {
        // all the points on a plane maps to the same normal
        return {0, 1, 0};
    }

    Aggregate Plane::local_intersect(const Ray &ray) {
        if (std::abs(ray.direction.y()) < math::epsilon) return {};

        // the formula works only if the plane is in xz (left-hand rule) and the
        // normal is pointing upwards (+y)
        auto t = -ray.origin.y() / ray.direction.y();

        return {{new Intersection{t, this}}};
    }

    std::ostream &operator<<(std::ostream &out, const Shape &shape) {
        return out << "{ type: " << shape.type() << " }";
    }
}