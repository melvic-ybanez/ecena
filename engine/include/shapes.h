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
        shape,
        sphere
    };

    std::ostream &operator<<(std::ostream &out, const Type &type);

    class Shape {
    public:
        math::Matrix<4, 4> transformation;

        Material material;

        Shape();

        virtual ~Shape() = default;

        Shape(const Shape &shape) = delete;

        Shape(Shape &&shape) noexcept = delete;

        Shape &operator=(const Shape &shape) = delete;

        Shape &operator=(Shape &&shape) noexcept = delete;

        virtual intersections::Aggregate intersect(const Ray &ray) const = 0;

        virtual Type type() const;

        virtual Vec normal_at(const Point &point) const = 0;

        Shape &translate(real x, real y, real z);

        Shape &scale(real x, real y, real z);

        Shape &rotate_x(real r);

        Shape &rotate_y(real r);

        Shape &rotate_z(real r);

        Shape &shear(real xy, real xz, real yx, real yz, real zx, real zy);
    };

    class Sphere : public Shape {
    public:
        intersections::Aggregate intersect(const Ray &ray) const override;

        Type type() const override;

        Vec normal_at(const Point &point) const override;
    };

    std::ostream &operator<<(std::ostream &out, const Shape &shape);
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
