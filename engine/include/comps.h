//
// Created by Melvic Ybanez on 12/31/22.
//

#ifndef ECENA_COMPS_H
#define ECENA_COMPS_H

#include "shapes.h"

namespace rt {
    struct Comps {
        real t;
        const Shape *object;
        Point point;
        Vec eye_vec;
        Vec normal_vec;
        bool inside;

        Comps(const Intersection &intersection, const Ray &ray);
    };
}

#endif //ECENA_COMPS_H
