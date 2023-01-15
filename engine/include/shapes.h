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

namespace rt::shapes {
    enum class Type {
        shape, sphere, plane, test, cube, cylinder, cone
    };

    std::ostream &operator<<(std::ostream &out, const Type &type);

    class Shape {
    public:
        Matrix<4, 4> transformation;

        std::unique_ptr<Material> material;

        Shape();

        virtual ~Shape() = default;

        Shape(const Shape &shape) = delete;

        Shape(Shape &&shape) noexcept = delete;

        Shape &operator=(const Shape &shape) = delete;

        Shape &operator=(Shape &&shape) noexcept = delete;

        Aggregate intersect(const Ray &ray);

        virtual Type type() const;

        Vec normal_at(const Point &point);

    protected:
        virtual Aggregate local_intersect(const Ray &local_ray) = 0;

        virtual Vec local_normal_at(const Point &local_point) = 0;
    };

    class Sphere : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;
    };

    class Plane : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;
    };

    class Cube : public Shape {
    public:
        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;

    private:
        std::array<real, 2> check_axis(real origin, real direction) const;
    };

    class CylinderLike: public Shape {
    public:
        constexpr static int radius_ = 1;

        real minimum;
        real maximum;
        bool closed;

        CylinderLike();

        CylinderLike(real minimum, real maximum, bool closed = false);

    protected:
        Aggregate intersect(const Ray &ray, real a, real b, real c);
        /**
         * Checks if the intersection at `t` is within the radius from the y-axis.
         */
        bool check_cap(const Ray &ray, real t) const;

        Aggregate &intersect_caps(const Ray &ray, Aggregate &xs);
    };

    class Cylinder : public CylinderLike {
    public:
        Cylinder() = default;

        Cylinder(real minimum, real maximum, bool closed = false);

        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;
    };

    class Cone : public CylinderLike {
    public:
        Cone() = default;

        Cone(real minimum, real maximum, bool closed = false);

        Type type() const override;

        Aggregate local_intersect(const Ray &ray) override;

        Vec local_normal_at(const Point &local_point) override;
    };

    std::ostream &operator<<(std::ostream &out, const Shape &shape);
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
