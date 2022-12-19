//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_SHAPES_H
#define ECENA_SHAPES_H

#include <vector>
#include "../../math/include/math_utils.h"
#include "ray.h"

namespace rt::shapes {
    using Intersections = std::vector<real>;

    enum class Type {
        shape,
        sphere
    };

    std::ostream &operator<<(std::ostream &out, const Type &type);

    class Shape {
    public:
        virtual Intersections intersect(const Ray &ray) const = 0;

        virtual Type type() const;

        virtual ~Shape() = default;
    };

    class Sphere : public Shape {
    public:
        Intersections intersect(const Ray &ray) const override;

        Type type() const override;
    };
}

namespace rt {
    using Shape = shapes::Shape;
}

#endif //ECENA_SHAPES_H
