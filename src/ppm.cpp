//
// Created by Melvic Ybanez on 12/10/22.
//
#include <sstream>

#include "../include/ppm.h"

namespace rt {
    Ppm::Ppm(const Canvas &canvas, const std::string &identifier, int max_color) :
            identifier_{identifier}, canvas{canvas}, max_color_{max_color} {}

    const std::string &Ppm::identifier() const {
        return identifier_;
    }

    std::string Ppm::header() const {
        return identifier() + "\n" + std::to_string(width()) + +" " + std::to_string(height()) + "\n" +
               std::to_string(max_color());
    }

    std::string Ppm::pixel_data() const {
        std::ostringstream out;
        for (const auto &row : canvas.pixels()) {
            int count = 0;
            for (const auto &pixel : row) {
                auto r = math::scale(pixel.red(), max_color_);
                auto g = math::scale(pixel.green(), max_color_);
                auto b = math::scale(pixel.blue(), max_color_);

                out << r << ' ' << g << ' ' << b;

                count++;
                if (count < width()) out << ' ';
            }
            out << std::endl;
        }
        return out.str();
    }

    int Ppm::width() const {
        return canvas.width();
    }

    int Ppm::height() const {
        return canvas.height();
    }

    int Ppm::max_color() const {
        return max_color_;
    }
}