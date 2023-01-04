//
// Created by Melvic Ybanez on 1/5/23.
//

#ifndef ECENA_PATTERNS_H
#define ECENA_PATTERNS_H

#include "../math/include/tuples.h"

namespace rt::patterns {
    class Stripe {
    public:
        Color first;
        Color second;

        Color at(const Point &point) const;
    };
}

#endif //ECENA_PATTERNS_H
