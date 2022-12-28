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

    intersections::Aggregate Sphere::intersect(const Ray &ray) const {
        auto transformed_ray = ray.transform(transformation.inverse());

        // compute the discriminant
        auto sphere_to_ray = transformed_ray.origin() - Point{0, 0, 0};
        auto a = transformed_ray.direction().dot(transformed_ray.direction());
        auto b = 2 * transformed_ray.direction().dot(sphere_to_ray);
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

    Vec Sphere::normal_at(const Point &world_point) const {
        auto inverse = transformation.inverse();
        auto object_point = inverse * world_point;
        auto object_normal = Vec{object_point - rt::Point{0, 0, 0}};
        auto world_normal = inverse.transpose() * object_normal;
        return Vec{world_normal}.normalize();
    }
}