//
// Created by Melvic Ybanez on 12/10/22.
//

#ifndef ECENA_PPM_H
#define ECENA_PPM_H

#include <string>
#include "canvas.h"

namespace rt {
    class Ppm {
        static const int max_line_width;
        std::string identifier_;
        Canvas canvas;
        int max_color_;

    public:
        explicit Ppm(const Canvas &canvas, const std::string &identifier = "P3", int max_color = 255);

        const std::string &identifier() const;

        std::string header() const;

        std::string pixel_data() const;

        int width() const;

        int height() const;

        int max_color() const;
    };

    std::ofstream &operator<<(std::ofstream &out, const Ppm &ppm);
}

#endif //ECENA_PPM_H
