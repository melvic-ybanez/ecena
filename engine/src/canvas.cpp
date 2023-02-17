//
// Created by Melvic Ybanez on 12/10/22.
//

#include <ostream>
#include "../include/canvas.h"
#include "../../shared/include/utils.h"

#define PARSE_INT(var_name) int var_name = 0; \
    if (auto maybe_val = read_int(); !maybe_val.has_value()) return std::nullopt; \
    else var_name = maybe_val.value(); \
    skip_comments()

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

    PixelRow& Canvas::operator[](int row) {
        return pixels[row];
    }

    Color Canvas::pixel_at(size_t col, size_t row) const {
        return pixels[row][col];
    }

    std::ostream& operator<<(std::ostream& out, const Canvas& canvas) {
        std::vector<std::string> row_str;
        for (const auto& row: canvas.pixels) {
            row_str.push_back(join_to_array(row));
        }

        return out << "{ pixels: " << join_to_array(row_str) << ", size: " << canvas.size << " }";
    }

    std::optional<Canvas> from_ppm(std::istream& is) {
        std::string line;

        auto skip_whitespace = [&] {
            while ((is.peek() == ' ' || is.peek() == '\n') && is.peek() != EOF) is.get();
        };

        auto skip_comments = [&] {
            skip_whitespace();
            while (is.peek() == '#') {
                std::getline(is, line);
                skip_whitespace();
            }
        };

        // read the first integer in the stream
        auto read_int = [&]() -> std::optional<int> {
            std::string int_str;

            while (is.peek() != ' ' && is.peek() != '\n' && is.peek() != EOF) {
                char c;
                is.get(c);
                int_str += c;
            }

            size_t pos;
            int value = std::stoi(int_str, &pos);

            if (pos != int_str.size()) return std::nullopt;
            return value;
        };

        skip_comments();
        if (!std::getline(is, line) || line != "P3") return std::nullopt;

        skip_comments();
        PARSE_INT(width);
        PARSE_INT(height);
        PARSE_INT(max_value);

        Canvas canvas{width, height};

        int r = 0;
        int c = 0;
        while (is.peek() != EOF) {
            PARSE_INT(x);
            PARSE_INT(y);
            PARSE_INT(z);

            canvas.pixels[r][c] = {
                    static_cast<real>(x) / max_value,
                    static_cast<real>(y) / max_value,
                    static_cast<real>(z) / max_value
            };

            c++;
            if (c == width) {
                c = 0;
                r++;
            }

        }

        return canvas;
    }
}