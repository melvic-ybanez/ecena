//
// Created by Melvic Ybanez on 12/10/22.
//

#ifndef ECENA_CANVAS_H
#define ECENA_CANVAS_H

#include <optional>
#include <vector>

#include "../math/include/math_utils.h"
#include "../math/include/tuples.h"

namespace rt::canvas {
    using PixelRow = std::vector<math::Color>;
    using PixelTable = std::vector<PixelRow>;

    class Canvas {
    public:
        PixelTable pixels;
        math::Dimensions size;

        Canvas();

        Canvas(int width, int height);

        [[nodiscard]] int width() const;

        [[nodiscard]] int height() const;

        PixelRow &operator[](int row);
    };

    std::ostream &operator<<(std::ostream &out, const Canvas &canvas);

    std::optional<Canvas> from_ppm(std::istream &is);
}

namespace rt {
    using PixelRow = canvas::PixelRow;
    using PixelTable = canvas::PixelTable;
    using Canvas = canvas::Canvas;
}

#endif //ECENA_CANVAS_H
