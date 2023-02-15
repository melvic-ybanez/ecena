//
// Created by Melvic Ybanez on 1/5/23.
//

#include <cmath>
#include <utility>
#include "../../engine/include/shapes.h"\


namespace rt::patterns {
    Pattern::Pattern() : transformation{matrix::identity<4, 4>()} {}

    Color Pattern::at(const Shape &shape, const Point &world_point) const {
        auto object_point = shape.world_to_object(world_point);
        auto pattern_point = transformation.inverse() * object_point;

        return at(pattern_point);
    }

    TwoComponentPattern::TwoComponentPattern(Color first, Color second) : first{std::move(first)},
                                                                          second{std::move(second)} {}

    Stripe::Stripe(Color first, Color second) : TwoComponentPattern(std::move(first), std::move(second)) {}

    Color Stripe::at(const Point &point) const {
        if (static_cast<int>(floor(point.x())) % 2 == 0) return first;
        return second;
    }

    Gradient::Gradient(Color first, Color second) : TwoComponentPattern(std::move(first), std::move(second)) {}

    /**
     * We are following this formula for the blending function:
     * `color(p, ca, cb) = ca + (cb - ca) * (px - floor(px))`, where `p` is a
     * point, `px` is the x component of point `p`, and `ca` and `cb` are the
     * components of this pattern.
     */
    Color Gradient::at(const Point &point) const {
        auto distance = second - first;
        auto fraction = point.x() - floor(point.x());
        return first + distance * fraction;
    }

    Ring::Ring(Color first, Color second) : TwoComponentPattern(std::move(first), std::move(second)) {}

    Color Ring::at(const Point &point) const {
        auto px2 = std::pow(point.x(), 2);
        auto pz2 = std::pow(point.z(), 2);
        if (static_cast<int>(floor(std::sqrt(px2 + pz2))) % 2 == 0) return first;
        return second;
    }

    Checkers::Checkers(Color first, Color second) : TwoComponentPattern(std::move(first), std::move(second)) {}

    Color Checkers::at(const Point &point) const {
        if (static_cast<int>(std::floor(point.x()) + std::floor(point.y()) + std::floor(point.z())) % 2 == 0)
            return first;
        return second;
    }

    TextureMap::TextureMap(std::unique_ptr<UV> uv, uv::Map map) : uv{std::move(uv)}, map{std::move(map)} {}

    Color TextureMap::at(const Point &point) const {
        auto [u, v] = map(point);
        return uv->at(u, v);
    }
}