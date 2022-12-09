//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/canvas.h"

namespace rt {
    Canvas::Canvas(int width, int height): width_(width), height_(height) {}

    int Canvas::width() const {
        return width_;
    }

    int Canvas::height() const {
        return height_;
    }

    PixelTable &Canvas::pixels() {
        return pixels_;
    }
}