//
// Created by Melvic Ybanez on 12/19/22.
//

#include <iostream>
#include "../include/intersection.h"

namespace rt::intersections {
    Intersection::Intersection(real t, const Shape *shape) : t_{t}, shape_{shape} {}

    Intersection::Intersection(const Intersection &from) :t_{from.t_}, shape_{from.shape_} {}

    Intersection::Intersection(Intersection &&from) noexcept : t_{from.t_}, shape_{from.shape_} {
        from.shape_ = nullptr;
    }

    Intersection::~Intersection() {
        delete shape_;
    }

    real Intersection::t() const {
        return t_;
    }

    const Shape *Intersection::shape() const {
        return shape_;
    }
}