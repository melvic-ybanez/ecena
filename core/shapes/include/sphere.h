//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_SPHERE_H
#define ECENA_SPHERE_H

#include "../../../math/include/matrix.h"
#include "shape_utils.h"
#include "../../include/ray.h"

namespace rt::shapes {
    class Sphere {
    public:
        Intersections intersect(const Ray &ray) const;
    };
}

#endif //ECENA_SPHERE_H
