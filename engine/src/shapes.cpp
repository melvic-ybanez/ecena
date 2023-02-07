//
// Created by Melvic Ybanez on 12/19/22.
//

#include <cmath>
#include <iostream>
#include <utility>
#include "../include/shapes.h"

namespace rt::shapes {
    std::ostream &operator<<(std::ostream &out, const Type &type) {
        std::array<std::string, 9> type_strings{
                "Shape", "Sphere", "Plane", "Test", "Cube", "Cylinder", "Cone", "Group", "Triangle"
        };
        auto index = static_cast<int>(type);
        return out << type_strings[index];
    }

    Type Shape::type() const {
        return Type::shape;
    }

    Shape::Shape() : transformation{math::matrix::identity<4, 4>()}, material(std::make_unique<Material>()),
                     parent{nullptr} {}

    Aggregate Shape::intersect(const Ray &ray) {
        auto local_ray = ray.transform(transformation.inverse());
        return local_intersect(local_ray);
    }

    Vec Shape::normal_at(const Point &world_point) {
        auto local_point = world_to_object(world_point);
        auto local_normal = local_normal_at(local_point);
        return normal_to_world(local_normal);
    }

    bool Shape::has_parent() const {
        return parent != nullptr;
    }

    Point Shape::world_to_object(const Point &point) const {
        return transformation.inverse() * (has_parent() ? parent->world_to_object(point) : point);
    }

    Vec Shape::normal_to_world(const Vec &local_normal) const {
        auto world_normal = Vec{transformation.inverse().transpose() * local_normal}.normalize();
        if (has_parent()) return parent->normal_to_world(world_normal);
        return world_normal;
    }

    Bounds Shape::parent_space_bounds() const {
        if (!cached_parent_space_bounds.has_value())
            cached_parent_space_bounds = bounds().transform(transformation);
        return cached_parent_space_bounds.value();
    }

    /**
     * Do not divide a primitive shape.
     */
    void Shape::divide(int threshold) {}

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
        return Cube::intersect(ray, this);
    }

    Aggregate Cube::intersect(const Ray &ray, Shape *cube_like) {
        auto [min, max] = cube_like->bounds();
        auto [x_t_min, x_t_max] = Cube::check_axis(ray.origin.x(), ray.direction.x(), min.x(), max.x());
        auto [y_t_min, y_t_max] = Cube::check_axis(ray.origin.y(), ray.direction.y(), min.y(), max.y());
        auto [z_t_min, z_t_max] = Cube::check_axis(ray.origin.z(), ray.direction.z(), min.z(), max.z());

        auto t_min = std::max({x_t_min, y_t_min, z_t_min});
        auto t_max = std::min({x_t_max, y_t_max, z_t_max});

        if (t_min > t_max) return {};

        return {{new Intersection{t_min, cube_like}, new Intersection{t_max, cube_like}}};
    }

    std::array<real, 2> Cube::check_axis(real origin, real direction, real min, real max) {
        auto t_min_numerator = min - origin;
        auto t_max_numerator = max - origin;

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

    CylinderLike::CylinderLike(real minimum, real maximum, bool closed) : min{minimum}, max{maximum},
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
        if (min < y0 && y0 < max) {
            xs.add(new Intersection{t0, this});
        }

        auto y1 = origin.y() + t1 * direction.y();
        if (min < y1 && y1 < max) {
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
        auto t = (min - ray.origin.y()) / ray.direction.y();
        if (check_cap(ray, t, min_limit())) xs.add(new Intersection{t, this});

        // intersection with the upper end cap (y = max)
        t = (max - ray.origin.y()) / ray.direction.y();
        if (check_cap(ray, t, max_limit())) xs.add(new Intersection{t, this});

        return xs;
    }

    Vec CylinderLike::normal_at(const Point &point, real y) {
        auto distance_from_y_squared = std::pow(point.x(), 2) + std::pow(point.z(), 2);
        if (distance_from_y_squared < 1 && point.y() >= max - math::epsilon) {
            return {0, 1, 0};
        }
        if (distance_from_y_squared < 1 && point.y() <= min + math::epsilon) {
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
        return min;
    }

    real Cone::max_limit() const {
        return max;
    }

    Type Group::type() const {
        return Type::group;
    }

    bool Group::empty() const {
        return children.empty();
    }

    void Group::add_child(Shape *shape) {
        add_child(std::unique_ptr<Shape>(shape));
    }

    void Group::add_child(std::unique_ptr<Shape> shape) {
        shape->parent = this;
        children.push_back(std::move(shape));
    }

    void Group::add_children(const std::vector<Shape *> &shapes) {
        for (auto shape: shapes) {
            add_child(shape);
        }
    }

    bool Group::contains(const Shape *shape) const {
        return std::find_if(children.begin(), children.end(), [&shape](auto &s) { return s.get() == shape; }) !=
               children.end();
    }

    Aggregate Group::local_intersect(const Ray &ray) {
        // If the ray does not intersect with the bounding box,
        // do not bother checking the children for intersections.
        if (Cube::intersect(ray, this).empty()) return {};

        Aggregate xs;
        for (auto &child: children) {
            auto xs_ = child->intersect(ray);
            xs.combine_with(xs_);
        }
        return xs;
    }

    Vec Group::local_normal_at(const Point &local_point) {
        throw std::runtime_error("Groups don't support this operation");
    }

    const Shape *Group::operator[](size_t i) const {
        return children[i].get();
    }

    std::ostream &operator<<(std::ostream &out, const Shape &shape) {
        out << "{ ";
        shape.display(out);
        return out << " }";
    }

    std::ostream &Shape::display(std::ostream &out) const {
        return out << "type: " << type() << ", material: " << *material;
    }

    Bounds Cube::bounds() const {
        return Bounds::cube();
    }

    Bounds Sphere::bounds() const {
        return Bounds::cube();
    }

    Bounds Plane::bounds() const {
        return {{-math::infinity, 0, -math::infinity}};
    }

    Bounds Cone::bounds() const {
        auto a = std::abs(min);
        auto b = std::abs(max);
        auto limit = std::max(a, b);
        return {{-limit, min, -limit},
                {limit,  max, limit}};
    }

    Bounds Cylinder::bounds() const {
        return {{-1, min, -1},
                {1,  max, 1}};
    }

    Bounds Group::bounds() const {
        if (cached_bounds.has_value()) return cached_bounds.value();

        Bounds bounds;
        for (auto &child: children) {
            bounds = bounds + child->parent_space_bounds();
        }
        cached_bounds = bounds;
        return bounds;
    }

    std::pair<std::vector<std::unique_ptr<Shape>>, std::vector<std::unique_ptr<Shape>>> Group::partition() {
        std::vector<std::unique_ptr<Shape>> left{};
        std::vector<std::unique_ptr<Shape>> right{};

        std::vector<std::unique_ptr<Shape>> new_children{};

        auto [left_bounds, right_bounds] = bounds().split();

        for (auto &child: children) {
            auto child_bounds = child->parent_space_bounds();

            if (left_bounds.contains(child_bounds)) {
                left.push_back(std::move(child));
            } else if (right_bounds.contains(child_bounds)) {
                right.push_back(std::move(child));
            } else {
                new_children.push_back(std::move(child));
            }
        }

        children = std::move(new_children);

        return {std::move(left), std::move(right)};
    }

    void Group::make_subgroup(std::vector<std::unique_ptr<Shape>> from_children) {
        std::unique_ptr<Group> subgroup = std::make_unique<Group>();
        for (auto &child: from_children) {
            subgroup->add_child(std::move(child));
        }
        add_child(std::move(subgroup));
    }

    void Group::divide(int threshold) {
        if (threshold <= count()) {
            auto [left, right] = partition();
            if (!left.empty()) make_subgroup(std::move(left));
            if (!right.empty()) make_subgroup(std::move(right));
        }

        for (auto &child: children) {
            child->divide(threshold);
        }
    }

    size_t Group::count() const {
        return children.size();
    }

    Triangle::Triangle(Point p1, Point p2, Point p3) : p1{std::move(p1)}, p2{std::move(p2)}, p3{std::move(p3)} {
        e1 = this->p2 - this->p1;
        e2 = this->p3 - this->p1;
        normal = e2.cross(e1).normalize();
    }

    Vec Triangle::local_normal_at(const Point &local_point) {
        return normal;
    }

    /**
     * This is an implementation of the Moller-Trumbore algorithm.
     */
    Aggregate Triangle::local_intersect(const Ray &ray) {
        auto dir_cross_e2 = ray.direction.cross(e2);
        auto determinant = e1.dot(dir_cross_e2);
        if (math::close_to_zero(determinant)) return {};

        auto f = 1.0 / determinant;
        auto p1_to_origin = Vec(ray.origin - p1);
        auto u = f * p1_to_origin.dot(dir_cross_e2);
        if (u < 0 || u > 1) return {};

        auto origin_cross_e1 = p1_to_origin.cross(e1);
        auto v = f * ray.direction.dot(origin_cross_e1);
        if (v < 0 || (u + v) > 1) return {};

        auto t = f * e2.dot(origin_cross_e1);
        return {{new Intersection{t, this}}};
    }

    Type Triangle::type() const {
        return Type::triangle;
    }

    /**
     * Constructs a bounding box by finding the smallest and largest components.
     */
    Bounds Triangle::bounds() const {
        Bounds box;
        return box + p1 + p2 + p3;
    }

    bool Triangle::operator==(const Triangle &that) const {
        return this->p1 == that.p1 && this->p2 == that.p2 && this->p3 == that.p3;
    }

    std::ostream &Triangle::display(std::ostream &out) const {
        out << "p1: " << p1 << ", p2: " << p2 << ", p3: " << p3;
        Shape::display(out);
        return out;
    }
}