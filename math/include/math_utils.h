//
// Created by Melvic Ybanez on 12/8/22.
//

#ifndef ECENA_MATH_UTILS_H
#define ECENA_MATH_UTILS_H

namespace rt::math {
    using real = double;

    inline constexpr double epsilon = 0.0001;

    constexpr double pi = 3.14159265358979323846;

    bool compare_reals(real a, real b);

    int scale(real value, int max_value);
}

namespace rt {
    using real = math::real;
}

#endif //ECENA_MATH_UTIILS
