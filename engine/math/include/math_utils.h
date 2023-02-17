//
// Created by Melvic Ybanez on 12/8/22.
//

#ifndef ECENA_MATH_UTILS_H
#define ECENA_MATH_UTILS_H

#include <limits>
#include <ostream>

namespace rt::math {
    using real = double;

    constexpr inline double epsilon = 0.0001;

    constexpr inline double pi = 3.14159265358979323846;

    constexpr inline real infinity = std::numeric_limits<real>::max();

    bool compare_reals(real a, real b);

    int scale(real value, int max_value);

    class Dimensions {
    public:
        real width;
        real height;

        Dimensions();

        Dimensions(real width, real height);

        Dimensions(int width, int height);

        bool operator==(const Dimensions& that) const;
    };

    std::ostream& operator<<(std::ostream& out, const Dimensions& dimensions);

    bool close_to_zero(real value);

    real pow2(real value);
}

namespace rt {
    using real = math::real;
}

#endif //ECENA_MATH_UTIILS
