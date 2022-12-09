//
// Created by Melvic Ybanez on 12/10/22.
//

#ifndef ECENA_CANVAS_H
#define ECENA_CANVAS_H

#include <vector>
#include "rtmath.h"
#include "tuples.h"

namespace rt {
    using PixelRow = std::vector<Color>;
    using PixelTable = std::vector<PixelRow>;
    class Canvas {
        PixelTable pixels_;
        int width_;
        int height_;
    public:
        Canvas(int width, int height);

        int width() const;

        int height() const;

        PixelTable &pixels();
    };
}

#endif //ECENA_CANVAS_H
