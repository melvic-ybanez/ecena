//
// Created by Melvic Ybanez on 12/10/22.
//

#ifndef ECENA_CANVAS_H
#define ECENA_CANVAS_H

#include <vector>
#include "../../math/include/math_utils.h"
#include "../../math/include/tuples.h"

namespace rt {
    using PixelRow = std::vector<math::Color>;
    using PixelTable = std::vector<PixelRow>;

    class Canvas {
    public:
        PixelTable pixels;
        math::Dimensions dimensions;

        Canvas();

        Canvas(int width, int height);

        int width() const;

        int height() const;

        PixelRow &operator[](int row);
    };
}

#endif //ECENA_CANVAS_H
