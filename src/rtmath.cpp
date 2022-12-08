//
// Created by Melvic Ybanez on 12/8/22.
//

#include "../include/rtmath.h"
#include <cstdlib>

namespace rt {
    bool compare_reals(real a, real b) {
        return std::abs(a - b) < EPSILON;
    }
}