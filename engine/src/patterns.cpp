//
// Created by Melvic Ybanez on 1/5/23.
//

#include <cmath>
#include "../../engine/include/shapes.h"\


namespace rt::patterns {
    Pattern::Pattern() : transformation{matrix::identity<4, 4>()} {}

    Stripe::Stripe(Color first, Color second) : first{std::move(first)}, second{std::move(second)} {}

    Color Stripe::at(const Point &point) const {
        if ((static_cast<int>(floor(point.x())) % 2) == 0) return first;
        return second;
    }

    Color Stripe::at(const Shape &shape, const Point &world_point) const {
        auto object_point = shape.transformation.inverse() * world_point;
        auto pattern_point = transformation.inverse() * object_point;

        return at(pattern_point);
    }
}