//
// Created by Melvic Ybanez on 1/5/23.
//

#include <cmath>
#include "../../engine/include/shapes.h"\


namespace rt::patterns {
    Pattern::Pattern() : transformation{matrix::identity<4, 4>()} {}

    Color Pattern::at(const Shape &shape, const Point &world_point) const {
        auto object_point = shape.transformation.inverse() * world_point;
        auto pattern_point = transformation.inverse() * object_point;

        return at(pattern_point);
    }

    Stripe::Stripe(Color first, Color second) : first{std::move(first)}, second{std::move(second)} {}

    Color Stripe::at(const Point &point) const {
        if ((static_cast<int>(floor(point.x())) % 2) == 0) return first;
        return second;
    }

    Gradient::Gradient(Color first, Color second) : first{std::move(first)}, second{std::move(second)} {}

    /**
     * We are following this formula for the blending function:
     * `color(p, ca, cb) = ca + (cb - ca) * (px - floor(px))`, where `p` is a
     * point, `px` is the x component of point `p`, and `ca` and `cb` are the
     * components of this pattern.
     */
    Color Gradient::at(const Point &point) const {
        auto distance = second - first;
        auto fraction = point.x() - static_cast<int>(floor(point.x()));
        return first + distance * fraction;
    }
}