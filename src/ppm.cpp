//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/ppm.h"

namespace rt {
    Ppm::Ppm(int width, int height, const std::string &identifier, int max_color) :
            identifier_{identifier}, width_{width}, height_{height}, max_color_{max_color} {}

    const std::string &Ppm::identifier() const {
        return identifier_;
    }

    int Ppm::width() const {
        return width_;
    }

    int Ppm::height() const {
        return height_;
    }

    int Ppm::max_color() const {
        return max_color_;
    }

    std::string Ppm::header() const {
        return identifier() + "\n" + std::to_string(width()) + +" " + std::to_string(height()) + "\n" +
               std::to_string(max_color());
    }
}