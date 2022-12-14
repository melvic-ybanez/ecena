//
// Created by Melvic Ybanez on 12/8/22.
//

#include "../include/math_utils.h"
#include <cstdlib>
#include <cmath>

namespace rt::math {
    Dimensions::Dimensions() : Dimensions(0, 0) {}

    Dimensions::Dimensions(real width, real height) : width{width}, height{height} {}

    Dimensions::Dimensions(int width, int height) :
            Dimensions(static_cast<double>(width),
                       static_cast<double>(height)) {}

    bool compare_reals(real a, real b) {
        auto result = std::abs(a - b);
        return std::isnan(result) || result < epsilon;
    }

    int scale(real value, int max_value) {
        auto result = static_cast<int>(std::floor((max_value + 1) * value));
        if (result > max_value) return max_value;
        if (result < 0) return 0;
        return result;
    }

    std::ostream &operator<<(std::ostream &out, const Dimensions &dimensions) {
        return out << "{ width: " << dimensions.width << ", height: " << dimensions.height << " }";
    }
}