//
// Created by Melvic Ybanez on 12/10/22.
//
#include <sstream>
#include <fstream>

#include "../include/ppm.h"

namespace rt {
    const int Ppm::max_line_width = 70;

    Ppm::Ppm(const Canvas& canvas, const std::string& identifier, int max_color) :
            identifier_{identifier}, canvas{canvas}, max_color_{max_color} {}

    const std::string& Ppm::identifier() const {
        return identifier_;
    }

    std::string Ppm::header() const {
        return identifier() + "\n" + std::to_string(width()) + +" " + std::to_string(height()) + "\n" +
               std::to_string(max_color());
    }

    std::string Ppm::pixel_data() const {
        std::string out;
        for (const auto& row: canvas.pixels) {
            std::vector<std::string> color_strs;

            int count = 0;
            for (const auto& pixel: row) {
                auto r = math::scale(pixel.red(), max_color_);
                auto g = math::scale(pixel.green(), max_color_);
                auto b = math::scale(pixel.blue(), max_color_);

                color_strs.push_back(std::to_string(r));
                color_strs.push_back(std::to_string(g));
                color_strs.push_back(std::to_string(b));

                count++;
                if (count == width()) color_strs.emplace_back("\n");
            }

            // make sure the rows don't go beyond the max line width
            size_t len = 0;
            for (const auto& color_str: color_strs) {
                if (color_str == "\n") {
                    out += color_str;
                    len = 0;
                    continue;
                }

                size_t color_size = color_str.size();

                if ((Ppm::max_line_width - len) < color_size + 1) {
                    out += "\n" + color_str;
                    len = color_size;
                } else if (len == 0) {
                    out += color_str;
                    len = color_size;
                } else {
                    out += " " + color_str;
                    len += color_size + 1;
                }
            }
        }

        return out;
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

    std::ostream& operator<<(std::ostream& out, const Ppm& ppm) {
        out << ppm.header() << "\n" << ppm.pixel_data();
        return out;
    }
}