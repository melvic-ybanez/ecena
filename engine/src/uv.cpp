//
// Created by Melvic Ybanez on 2/16/23.
//

#include <cmath>

#include "../include/uv.h"

namespace rt::uv {
    Image::Image(Canvas canvas) : canvas{std::move(canvas)} {}

    Color Image::at(real u, real v) const {
        v = 1 - v;  // v goes from the bottom to the top, so we need to flip it
        auto x = u * (canvas.width() - 1);
        auto y = v * (canvas.height() - 1);

        return canvas.pixel_at(static_cast<size_t>(std::round(x)), static_cast<size_t>(std::round(y)));
    }
}

namespace rt::uv::maps {
    std::pair<real, real> spherical(const Point &point) {
        // they call this azimuthal angle. range: (-pi, pi]
        auto theta = std::atan2(point.x(), point.z());

        // the vector from the origin to `point`
        // note: this is equal to the sphere's radius since the sphere is assumed to be
        // centered at the origin
        Vec vec{point.x(), point.y(), point.z()};

        auto radius = vec.magnitude();

        // the polar angle. range: [0, pi]
        auto phi = std::acos(point.y() / radius);

        auto u = theta / (2 * math::pi);    // range: (-0.5, 0.5]
        u += 0.5;    // now it's [0, 1)
        u = 1 - u;   // to make it counterclockwise, since `theta` goes clockwise when viewed from above

        // v should go from 0 to 1 as it moves from south to north, so we subtract it from 1 as well
        auto v = 1 - phi / math::pi;

        return {u, v};
    }
}