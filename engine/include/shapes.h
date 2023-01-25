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
        shape, sphere, plane, test, cube, cylinder, cone, group
    };

    std::ostream &operator<<(std::ostream &out, const Type &type);

    class Shape {
    public:
        Matrix<4, 4> transformation;

        std::unique_ptr<Material> material;

        Shape *parent;

        Shape();

        virtual ~Shape() = default;

        Shape(const Shape &shape) = delete;

        Shape(Shape &&shape) noexcept = delete;

        Shape &operator=(const Shape &shape) = delete;

        Shape &operator=(Shape &&shape) noexcept = delete;

        virtual Bounds bounds() const = 0;

        virtual Type type() const;

        Aggregate intersect(const Ray &ray);

        Vec normal_at(const Point &world_point);

        Point world_to_object(const Point &point) const;

        Vec normal_to_world(const Vec &local_normal) const;

        bool has_parent() const;

        Bounds parent_space_bounds() const;

    protected:
        virtual Aggregate local_intersect(const Ray &local_ray) = 0;

        virtual Vec local_normal_at(const Point &local_point) = 0;

    private:
        mutable std::optional<Bounds> cached_parent_space_bounds;
    };

    class Sphere : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

        Bounds bounds() const override;
    };

    class Plane : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

        Bounds bounds() const override;
    };

    class Cube : public Shape {
    public:
        static Aggregate intersect(const Ray &ray, Shape *cube_like);

        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

        Bounds bounds() const override;

    private:
        static std::array<real, 2> check_axis(real origin, real direction, real min, real max);
    };

    class CylinderLike: public Shape {
    public:
        real min;
        real max;
        bool closed;

        CylinderLike();

        CylinderLike(real minimum, real maximum, bool closed = false);

        virtual ~CylinderLike() = default;

        CylinderLike(const CylinderLike &from) = delete;

        CylinderLike(CylinderLike &&from) = delete;

        CylinderLike &operator=(const CylinderLike &from) = delete;

        CylinderLike &operator=(CylinderLike &&from) = delete;

    protected:
        Aggregate intersect(const Ray &ray, real a, real b, real c);

        Vec normal_at(const Point &point, real y);

        /**
         * Checks if the intersection at `t` is within the radius from the y-axis.
         */
        bool check_cap(const Ray &ray, real t, real limit) const;

        Aggregate &intersect_caps(const Ray &ray, Aggregate &xs);

        virtual real min_limit() const = 0;

        virtual real max_limit() const = 0;
    };

    class Cylinder : public CylinderLike {
    public:
        Cylinder() = default;

        Cylinder(real minimum, real maximum, bool closed = false);

        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

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

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

        Bounds bounds() const override;

    protected:
        real min_limit() const override;

        real max_limit() const override;
    };

    class Group : public Shape {
    public:
        std::vector<std::unique_ptr<Shape>> children;

        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

        bool empty() const;

        void add_child(Shape *shape);

        void add_child(std::unique_ptr<Shape> shape);

        void add_children(const std::vector<Shape *>& shapes);

        bool contains(const Shape *shape) const;

        Bounds bounds() const override;

        std::pair<std::unique_ptr<Group>, std::unique_ptr<Group>> partition();

    private:
        mutable std::optional<Bounds> cached_bounds;
    };

    std::ostream &operator<<(std::ostream &out, const Shape &shape);
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
