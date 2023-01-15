//
// Created by Melvic Ybanez on 12/19/22.
//

#include <cmath>
#include <iostream>
#include "../include/shapes.h"

namespace rt::shapes {
    Shape::Shape() : transformation{math::matrix::identity<4, 4>()}, material(std::make_unique<Material>()) {}

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

    Type Cube::type() const {
        return Type::cube;
    }

    Aggregate Cube::local_intersect(const Ray &ray) {
        auto [x_t_min, x_t_max] = check_axis(ray.origin.x(), ray.direction.x());
        auto [y_t_min, y_t_max] = check_axis(ray.origin.y(), ray.direction.y());
        auto [z_t_min, z_t_max] = check_axis(ray.origin.z(), ray.direction.z());

        auto t_min = std::max({x_t_min, y_t_min, z_t_min});
        auto t_max = std::min({x_t_max, y_t_max, z_t_max});

        if (t_min > t_max) return {};

        return {{new Intersection{t_min, this}, new Intersection{t_max, this}}};
    }

    std::array<real, 2> Cube::check_axis(real origin, real direction) const {
        auto t_min_numerator = (-1 - origin);
        auto t_max_numerator = (1 - origin);

        auto t_min = t_min_numerator * math::infinity;
        auto t_max = t_max_numerator * math::infinity;

        if (std::abs(direction) >= math::epsilon) {
            t_min = t_min_numerator / direction;
            t_max = t_max_numerator / direction;
        }

        if (t_min > t_max) std::swap(t_min, t_max);

        return {t_min, t_max};
    }

    Vec Cube::local_normal_at(const Point &local_point) {
        auto max_c = std::max({std::abs(local_point.x()), std::abs(local_point.y()), std::abs(local_point.z())});

        if (max_c == std::abs(local_point.x())) {
            return {local_point.x(), 0, 0};
        }
        if (max_c == std::abs(local_point.y())) {
            return {0, local_point.y(), 0};
        }

        return {0, 0, local_point.z()};
    }

    CylinderLike::CylinderLike() : CylinderLike(-math::infinity, math::infinity) {}

    CylinderLike::CylinderLike(real minimum, real maximum, bool closed) : minimum{minimum}, maximum{maximum},
                                                                          closed{closed} {}

    Aggregate CylinderLike::intersect(const Ray &ray, real a, real b, real c) {
        auto [origin, direction] = ray;

        auto discriminant = b * b - 4 * a * c;

        // no intersections
        if (discriminant < 0) return {};

        auto t0 = (-b - std::sqrt(discriminant)) / (2 * a);
        auto t1 = (-b + std::sqrt(discriminant)) / (2 * a);
        if (t0 > t1) std::swap(t0, t1);

        Aggregate xs{};

        auto y0 = origin.y() + t0 * direction.y();
        if (minimum < y0 && y0 < maximum) {
            xs.add(new Intersection{t0, this});
        }

        auto y1 = origin.y() + t1 * direction.y();
        if (minimum < y1 && y1 < maximum) {
            xs.add(new Intersection{t1, this});
        }

        return std::move(intersect_caps(ray, xs));
    }

    bool CylinderLike::check_cap(const Ray &ray, real t, real limit) const {
        auto x = ray.origin.x() + t * ray.direction.x();
        auto z = ray.origin.z() + t * ray.direction.z();
        return (x * x + z * z) <= std::abs(limit);
    }

    Aggregate &CylinderLike::intersect_caps(const Ray &ray, Aggregate &xs) {
        if (!closed || math::close_to_zero(ray.direction.y())) return xs;

        // intersection with the lower end cap (y = min)
        auto t = (minimum - ray.origin.y()) / ray.direction.y();
        if (check_cap(ray, t, min_limit())) xs.add(new Intersection{t, this});

        // intersection with the upper end cap (y = max)
        t = (maximum - ray.origin.y()) / ray.direction.y();
        if (check_cap(ray, t, max_limit())) xs.add(new Intersection{t, this});

        return xs;
    }

    Vec CylinderLike::normal_at(const Point &point, real y) {
        auto distance_from_y_squared = std::pow(point.x(), 2) + std::pow(point.z(), 2);
        if (distance_from_y_squared < 1 && point.y() >= maximum - math::epsilon) {
            return {0, 1, 0};
        }
        if (distance_from_y_squared < 1 && point.y() <= minimum + math::epsilon) {
            return {0, -1, 0};
        }

        return {point.x(), y, point.z()};
    }

    Cylinder::Cylinder(real minimum, real maximum, bool closed) : CylinderLike(minimum, maximum, closed) {}

    Type Cylinder::type() const {
        return Type::cylinder;
    }

    Aggregate Cylinder::local_intersect(const Ray &ray) {
        auto [origin, direction] = ray;
        auto a = math::pow2(direction.x()) + math::pow2(direction.z());

        // ray is parallel to the y-axis
        if (math::close_to_zero(a)) {
            Aggregate aggregate;
            return std::move(intersect_caps(ray, aggregate));
        }

        auto b = 2 * origin.x() * direction.x() + 2 * ray.origin.z() * ray.direction.z();
        auto c = math::pow2(origin.x()) + math::pow2(origin.z()) - 1;

        return CylinderLike::intersect(ray, a, b, c);
    }

    Vec Cylinder::local_normal_at(const Point &local_point) {
        return CylinderLike::normal_at(local_point, 0);
    }

    real Cylinder::min_limit() const {
        return 1;
    }

    real Cylinder::max_limit() const {
        return 1;
    }

    Cone::Cone(real minimum, real maximum, bool closed) : CylinderLike(minimum, maximum, closed) {}

    Type Cone::type() const {
        return Type::cone;
    }

    Aggregate Cone::local_intersect(const Ray &local_ray) {
        auto [o, d] = local_ray;
        auto a = math::pow2(d.x()) - math::pow2(d.y()) + math::pow2(d.z());
        auto b = 2 * o.x() * d.x() - 2 * o.y() * d.y() + 2 * o.z() * d.z();
        auto c = math::pow2(o.x()) - math::pow2(o.y()) + math::pow2(o.z());

        if (math::close_to_zero(a)) {
            if (math::close_to_zero(b)) return {};

            auto t = -c / (2.0 * b);
            return {{new Intersection{t, this}}};
        }

        return CylinderLike::intersect(local_ray, a, b, c);
    }

    Vec Cone::local_normal_at(const Point &local_point) {
        auto y = std::sqrt(math::pow2(local_point.x()) + math::pow2(local_point.z()));
        if (local_point.y() > 0) y = -y;
        return CylinderLike::normal_at(local_point, y);
    }

    real Cone::min_limit() const {
        return minimum;
    }

    real Cone::max_limit() const {
        return maximum;
    }

    std::ostream &operator<<(std::ostream &out, const Shape &shape) {
        return out << "{ type: " << shape.type() << ", material: " << *shape.material << " }";
    }
}