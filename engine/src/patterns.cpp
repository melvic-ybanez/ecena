//
// Created by Melvic Ybanez on 1/5/23.
//

#include <cmath>
#include "../include/patterns.h"

namespace rt::patterns {
    Color Stripe::at(const Point &point) const {
        if ((static_cast<int>(floor(point.x())) % 2) == 0) return first;
        return second;
    }
}