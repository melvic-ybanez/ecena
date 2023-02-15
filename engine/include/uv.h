//
// Created by Melvic Ybanez on 2/16/23.
//

#ifndef ECENA_UV_H
#define ECENA_UV_H

#include "../math/include/tuples.h"
#include "canvas.h"

namespace rt::uv {
    using Map = std::function<std::pair<real, real>(const Point &)>;

    class UV {
    public:
        [[nodiscard]] virtual Color at(real u, real v) const = 0;

        UV() = default;

        virtual ~UV() = default;

        UV(const UV &that) = delete;

        UV(UV &&that) = delete;

        UV &operator=(const UV &that) = delete;

        UV &operator=(UV &&that) = delete;
    };

    class Image : public UV {
    public:
        Canvas canvas;

        Image(Canvas canvas);

        Color at(real u, real v) const override;
    };
}

namespace rt::uv::maps {
    std::pair<real, real> spherical(const Point &point);
}

namespace rt {
    using UV = uv::UV;
}

#endif //ECENA_UV_H
