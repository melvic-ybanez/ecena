//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/canvas.h"

namespace rt {
    Canvas::Canvas() : Canvas(0, 0) {}

    Canvas::Canvas(int width, int height) : dimensions{width, height} {
        pixels.insert(pixels.end(), height, PixelRow(width, math::Color::black_));
    }

    int Canvas::width() const {
        return dimensions.width;
    }

    int Canvas::height() const {
        return dimensions.height;
    }

    PixelRow &Canvas::operator[](int row){
        return pixels[row];
    }
}