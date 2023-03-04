//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_SHAPES_H
#define ECENA_SHAPES_H

#include <vector>
#include "../math/include/math_utils.h"
#include "ray.h"
#include "intersection.h"
#include "materials.h"
#include "bounds.h"

namespace rt::shapes {
    enum class Type {
        shape, sphere, plane, test, cube, cylinder, cone, group, triangle
    };

    std::ostream& operator<<(std::ostream& out, const Type& type);

    class Shape {
    public:
        Matrix<4, 4> transformation;

        Material* material;

        Shape* parent;

        Shape(Material* material = new Material);

        virtual ~Shape() = default;

        Shape(const Shape& shape) = delete;

        Shape(Shape&& shape) noexcept = delete;

        Shape& operator=(const Shape& shape) = delete;

        Shape& operator=(Shape&& shape) noexcept = delete;

        virtual Bounds bounds() const = 0;

        virtual Type type() const;

        virtual void divide(int threshold);

        virtual std::ostream& display(std::ostream& out) const;

        virtual bool operator==(const Shape& other) const;

        Aggregate intersect(const Ray& ray);

        Vec normal_at(const Point& world_point, const Intersection* hit = nullptr);

        Point world_to_object(const Point& point) const;

        Vec normal_to_world(const Vec& local_normal) const;

        bool has_parent() const;

        Bounds parent_space_bounds() const;

    protected:
        virtual Aggregate local_intersect(const Ray& local_ray) = 0;

        // These two `local_normal_at` functions have default implementations, in which each is
        // defined in terms of the other. This means a subclass is only required to override
        // one of them, and the other will automatically be derived.
        virtual Vec local_normal_at(const Point& local_point, const Intersection* hit);

        virtual Vec local_normal_at(const Point& local_point);

    private:
        mutable std::optional<Bounds> cached_parent_space_bounds;
    };

    class Sphere : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;
    };

    class Plane : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;
    };

    class Cube : public Shape {
    public:
        static Aggregate intersect(const Ray& ray, Shape* cube_like);

        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;

    private:
        static std::array<real, 2> check_axis(real origin, real direction, real min, real max);
    };

    class CylinderLike : public Shape {
    public:
        real min;   // the lower value at which this shape is truncated
        real max;   // the upper value at which this shape is truncated
        bool closed;

        CylinderLike();

        CylinderLike(real minimum, real maximum, bool closed = false);

    protected:
        Aggregate intersect(const Ray& ray, real a, real b, real c);

        Vec normal_at(const Point& point, real y);

        /**
         * Checks if the intersection at `t` is within the radius from the y-axis.
         */
        bool check_cap(const Ray& ray, real t, real limit) const;

        Aggregate& intersect_caps(const Ray& ray, Aggregate& xs);

        virtual real min_limit() const = 0;

        virtual real max_limit() const = 0;
    };

    class Cylinder : public CylinderLike {
    public:
        Cylinder() = default;

        Cylinder(real minimum, real maximum, bool closed = false);

        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;

    protected:
        real min_limit() const override;

        real max_limit() const override;
    };

    class Cone : public CylinderLike {
    public:
        Cone() = default;

        Cone(real minimum, real maximum, bool closed = false);

        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;

    protected:
        real min_limit() const override;

        real max_limit() const override;
    };

    class Group : public Shape {
    public:
        std::vector<std::unique_ptr<Shape>> children;

        Type type() const override;

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        bool empty() const;

        void add_child(Shape* shape);

        void add_child(std::unique_ptr<Shape> shape);

        void add_children(const std::vector<Shape*>& shapes);

        bool contains_ptr(const Shape* shape) const;

        bool contains_val(const Shape& shape) const;

        Bounds bounds() const override;

        /**
         * Groups the children according to which side (left or right) of the bounds they belong and
         * return them. It keeps the ones that do not belong to either side (i.e. the shapes that
         * partially belong to both sides).
         */
        std::pair<std::vector<std::unique_ptr<Shape>>, std::vector<std::unique_ptr<Shape>>> partition();

        /**
         * Creates a subgroup with the given children. The created subgroup becomes a member of this group.
         */
        void make_subgroup(std::vector<std::unique_ptr<Shape>> children);

        void divide(int threshold) override;

        size_t count() const;

        const Shape* operator[](size_t i) const;

    private:
        mutable std::optional<Bounds> cached_bounds;
    };

    class NamedGroup : public Group {
    public:
        std::string name;

        NamedGroup(std::string name);

        bool operator==(const Shape& other) const override;
    };

    class Triangle : public Shape {
    public:
        // The points of the triangle
        Point p1;
        Point p2;
        Point p3;

        // The edges
        Vec e1;
        Vec e2;

        // Precomputed normal
        Vec normal;

        Triangle(Point p1, Point p2, Point p3, Material* material = new Material);

        Aggregate local_intersect(const Ray& ray) override;

        Vec local_normal_at(const Point& local_point) override;

        Bounds bounds() const override;

        Type type() const override;

        bool operator==(const Shape& other) const override;

        virtual std::ostream& display(std::ostream& out) const override;
    };

    class SmoothTriangle : public Triangle {
    public:
        // normal vectors
        Vec n1;
        Vec n2;
        Vec n3;

        SmoothTriangle(Point p1, Point p2, Point p3, Vec n1, Vec n2, Vec n3);

        SmoothTriangle(Point p1, Point p2, Point p3);

        Vec local_normal_at(const Point& local_point, const Intersection* hit) override;

        bool operator==(const Shape& other) const override;

        std::ostream& display(std::ostream& out) const override;
    };

    std::ostream& operator<<(std::ostream& out, const Shape& shape);
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
