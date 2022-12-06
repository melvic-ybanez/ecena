//
// Created by Melvic Ybanez on 12/4/22.
//

#include "../include/color.h"

void write_color(std::ofstream &out, rt::Color pixel_color) {
    auto r = static_cast<int>(255.999 * pixel_color.x());
    auto g = static_cast<int>(255.999 * pixel_color.y());
    auto b = static_cast<int>(255.999 * pixel_color.z());

    out << r << ' ' << g << ' ' << b << std::endl;
}