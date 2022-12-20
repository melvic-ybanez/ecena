//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_INTERSECTION_H
#define ECENA_INTERSECTION_H

#include <vector>
#include "../../math/include/math_utils.h"

namespace rt::shapes {
    class Shape;
}

namespace rt {
    using Shape = shapes::Shape;
}

namespace rt::intersections {
    class Intersection {
        real t_;
        const Shape *shape_;

    public:
        Intersection(real t, const Shape *shape);

        Intersection(const Intersection &from);

        Intersection(Intersection &&from) noexcept;

        ~Intersection();

        real t() const;

        const Shape *shape() const;
    };

    using Set = std::vector<Intersection *>;
}

namespace rt {
    using Intersection = intersections::Intersection;
}

#endif //ECENA_INTERSECTION_H
