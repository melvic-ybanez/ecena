//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/canvas.h"

namespace rt {
    Canvas::Canvas(int width, int height) : width_(width), height_(height) {
        pixels_.insert(pixels_.end(), height, PixelRow(width, math::Color::black_));
    }

    int Canvas::width() const {
        return width_;
    }

    int Canvas::height() const {
        return height_;
    }

    const PixelTable &Canvas::pixels() const {
        return pixels_;
    }

    PixelRow &Canvas::operator[](int row){
        return pixels_[row];
    }
}