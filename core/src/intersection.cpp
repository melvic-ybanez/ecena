//
// Created by Melvic Ybanez on 12/19/22.
//

#include "../include/intersection.h"

namespace rt::intersections {
    Intersection::Intersection(real t, shapes::Shape *shape) : t_{t}, shape_{shape} {}

    Intersection::~Intersection() {
        delete shape_;
    }

    real Intersection::t() const {
        return t_;
    }

    Shape *Intersection::shape() const {
        return shape_;
    }
}