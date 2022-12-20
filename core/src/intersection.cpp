//
// Created by Melvic Ybanez on 12/19/22.
//

#include "../include/intersection.h"

namespace rt::intersections {
    Intersection::Intersection(real t, const Shape *shape) : t_{t}, shape_{shape} {}

    real Intersection::t() const {
        return t_;
    }

    const Shape *Intersection::shape() const {
        return shape_;
    }
}