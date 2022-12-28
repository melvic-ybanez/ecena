//
// Created by Melvic Ybanez on 12/10/22.
//

#include <ostream>
#include "../include/canvas.h"
#include "../../shared/include/utils.h"

namespace rt {
    Canvas::Canvas() : Canvas(1, 1) {}

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

    std::ostream &operator<<(std::ostream &out, const Canvas &canvas) {
        std::vector<std::string> row_str;
        for (const auto &row : canvas.pixels) {
            row_str.push_back(join_array(row));
        }

        return out << "{ pixels: " << join_array(row_str) << ", dimensions: " << canvas.dimensions << " }";
    }
}