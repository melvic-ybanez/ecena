//
// Created by Melvic Ybanez on 12/8/22.
//

#ifndef ECENA_UTILS_H
#define ECENA_UTILS_H

namespace rt {
    using real = double;
}

namespace rt::math {
    constexpr double epsilon = 0.0001;

    constexpr double pi = 3.14159265358979323846;

    bool compare_reals(real a, real b);

    int scale(real value, int max_value);
}

#endif //ECENA_UTILS_H
