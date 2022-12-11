//
// Created by Melvic Ybanez on 12/8/22.
//

#ifndef ECENA_UTILS_H
#define ECENA_UTILS_H

namespace rt {
    using real = double;
}

namespace rt::math {
    const double EPSILON = 0.000001;

    bool compare_reals(real a, real b);

    int scale(real value, int max_value);
}

#endif //ECENA_UTILS_H
