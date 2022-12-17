//
// Created by Melvic Ybanez on 12/17/22.
//

#ifndef ECENA_RAY_H
#define ECENA_RAY_H

#include "tuples.h"

namespace rt {
    class Ray {
        const Point &origin_;
        const Vec &direction_;

    public:
        Ray(const Point &origin, const Vec &direction);

        const Point &origin() const;

        const Vec &direction() const;
    };
}

#endif //ECENA_RAY_H
