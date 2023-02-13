//
// Created by Melvic Ybanez on 12/10/22.
//

#include <ostream>
#include "../include/canvas.h"
#include "../../shared/include/utils.h"

#define PARSE_INT_SEP(is, value, sep) if (!read_line(is, sep)) return std::nullopt; \
    int value = std::stoi(line, &pos); \
    if (pos != line.size()) return std::nullopt
#define PARSE_INT(is, value) PARSE_INT_SEP(is, value, ' ')
#define PARSE_INT_LINE(is, value) PARSE_INT_SEP(is, value, '\n')

namespace rt::canvas {
    Canvas::Canvas() : Canvas(1, 1) {}

    Canvas::Canvas(int width, int height) : size{width, height} {
        pixels.insert(pixels.end(), height, PixelRow(width, math::Color::black_));
    }

    int Canvas::width() const {
        return static_cast<int>(size.width);
    }

    int Canvas::height() const {
        return static_cast<int>(size.height);
    }

    PixelRow &Canvas::operator[](int row) {
        return pixels[row];
    }

    std::ostream &operator<<(std::ostream &out, const Canvas &canvas) {
        std::vector<std::string> row_str;
        for (const auto &row: canvas.pixels) {
            row_str.push_back(join_to_array(row));
        }

        return out << "{ pixels: " << join_to_array(row_str) << ", size: " << canvas.size << " }";
    }

    std::optional<Canvas> from_ppm(std::istream &is) {
        std::string line;
        size_t pos;

        auto read_line = [&](std::istream &is, char sep) -> std::istream &{
            while (is.peek() == '#') std::getline(is, line);
            return std::getline(is, line, sep);
        };

        if (!read_line(is, '\n') || line != "P3") return std::nullopt;

        PARSE_INT(is, width);
        PARSE_INT_LINE(is, height);
        PARSE_INT_LINE(is, max_value);

        Canvas canvas{width, height};

        int i = 0;
        while (read_line(is, '\n')) {
            std::stringstream row{line};
            while (row.peek() != EOF) {
                PARSE_INT(row, x);
                PARSE_INT(row, y);
                PARSE_INT(row, z);

                int r = i / width;
                int c = i % width;

                canvas.pixels[r][c] = {
                        static_cast<real>(x) / max_value,
                        static_cast<real>(y) / max_value,
                        static_cast<real>(z) / max_value
                };

                i++;
                while (row.peek() == ' ') row.get();
            }
        }

        return canvas;
    }
}