//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_INTERSECTION_H
#define ECENA_INTERSECTION_H

#include "../../math/include/math_utils.h"
#include "shapes.h"

namespace rt::intersections {
    class Intersection {
        real t_;
        Shape *shape_;

    public:
        Intersection(real t, shapes::Shape *shape);

        ~Intersection();

        real t() const;

        Shape *shape() const;
    };
}

namespace rt {
    using Intersection = intersections::Intersection;
}

#endif //ECENA_INTERSECTION_H
