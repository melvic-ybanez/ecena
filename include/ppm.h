//
// Created by Melvic Ybanez on 12/10/22.
//

#ifndef ECENA_PPM_H
#define ECENA_PPM_H

#include <string>

namespace rt {
    class Ppm {
        const std::string &identifier_;
        int width_;
        int height_;
        int max_color_;

    public:
        Ppm(int width, int height, const std::string &identifier = "P3", int max_color = 255);

        const std::string &identifier() const;

        int width() const;

        int height() const;

        int max_color() const;

        std::string header() const;
    };
}

#endif //ECENA_PPM_H
