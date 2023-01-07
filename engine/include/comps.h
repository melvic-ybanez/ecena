//
// Created by Melvic Ybanez on 12/31/22.
//

#ifndef ECENA_COMPS_H
#define ECENA_COMPS_H

#include "shapes.h"

namespace rt::comps {
    struct Comps {
        real t{};
        Shape *object{};
        Point point;
        Point over_point;
        Vec eye_vec;
        Vec normal_vec;
        Vec reflect_vec;
        bool inside{};
        real n1;
        real n2;
    };

    Comps prepare(const Intersection &hit, const Ray &ray, Aggregate &aggregate);

    Comps prepare(const Intersection &hit, const Ray &ray);
}

namespace rt {
    using Comps = comps::Comps;
}

#endif //ECENA_COMPS_H
