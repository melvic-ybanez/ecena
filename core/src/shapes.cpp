//
// Created by Melvic Ybanez on 12/19/22.
//

#include "../include/shapes.h"
#include "../include/world.h"

namespace rt::shapes {
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

    Intersections Sphere::intersect(const Ray &ray) const {
        // compute the discriminant
        auto sphere_to_ray{ray.origin() - world::origin};
        auto a{ray.direction().dot(ray.direction())};
        auto b{2 * ray.direction().dot(sphere_to_ray)};
        auto c{Vec{sphere_to_ray}.dot(sphere_to_ray) - 1};
        auto discriminant{b * b - 4 * a * c};

        if (discriminant < 0) return {};

        auto t1{(-b - std::sqrt(discriminant)) / (2 * a)};
        auto t2{(-b + std::sqrt(discriminant)) / (2 * a)};

        return {t1, t2};
    }

    Type Sphere::type() const {
        return Type::sphere;
    }
}