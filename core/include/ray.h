//
// Created by Melvic Ybanez on 12/17/22.
//

#ifndef ECENA_RAY_H
#define ECENA_RAY_H

#include "tuples.h"

namespace rt {
    class Ray {
        Point origin_;
        Vec direction_;

    public:
        Ray(const Point &origin, const Vec &direction);

        Ray(Point &&origin, Vec &&direction);

        const Point &origin() const;

        const Vec &direction() const;

        Point at(real t) const;
    };
}

#endif //ECENA_RAY_H
