//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/canvas.h"

namespace rt {
    Canvas::Canvas(int width, int height) : width_(width), height_(height) {
        pixels_.insert(pixels_.end(), height, PixelRow(width, Color::black_));
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

    const Color &Canvas::pixel_at(int row, int col) const {
        return pixels()[row][col];
    }

    void Canvas::write_pixel(int row, int col, const Color &color) {
        pixels_[row][col] = color;
    }
}