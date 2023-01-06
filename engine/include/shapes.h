//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_SHAPES_H
#define ECENA_SHAPES_H

#include <vector>
#include "../math/include/math_utils.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"

namespace rt::shapes {
    enum class Type {
        shape, sphere, plane, test
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

    std::ostream &operator<<(std::ostream &out, const Shape &shape);
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
