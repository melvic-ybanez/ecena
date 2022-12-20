//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_SHAPES_H
#define ECENA_SHAPES_H

#include <vector>
#include "../../math/include/math_utils.h"
#include "ray.h"
#include "intersection.h"

namespace rt::shapes {
    enum class Type {
        shape,
        sphere
    };

    std::ostream &operator<<(std::ostream &out, const Type &type);

    class Shape {
    public:
        Shape();

        virtual ~Shape();

        Shape(const Shape &shape);

        Shape(Shape &&shape) noexcept;

        Shape &operator=(const Shape &shape);

        Shape &operator=(Shape &&shape) noexcept;

        virtual intersections::Set intersect(const Ray &ray) const = 0;

        virtual Type type() const;
    };

    class Sphere : public Shape {
    public:
        intersections::Set intersect(const Ray &ray) const override;

        Type type() const override;
    };
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
