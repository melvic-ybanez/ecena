//
// Created by Melvic Ybanez on 12/19/22.
//

#include <cmath>
#include <iostream>
#include "../include/shapes.h"

namespace rt::shapes {
    Shape::Shape() : transformation{math::matrix::identity<4, 4>()} {}

    Shape::Shape(const Shape &) = default;

    Shape::~Shape() = default;

    Shape::Shape(Shape &&) noexcept = default;

    Shape &Shape::operator=(const Shape &) = default;

    Shape &Shape::operator=(Shape &&) noexcept = default;

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
        auto discriminant{b * b - 4 * a * c};

        if (discriminant < 0) return {};

        auto t1{(-b - std::sqrt(discriminant)) / (2 * a)};
        auto t2{(-b + std::sqrt(discriminant)) / (2 * a)};

        return {{new Intersection{t1, this}, new Intersection{t2, this}}};
    }

    Type Sphere::type() const {
        return Type::sphere;
    }
}