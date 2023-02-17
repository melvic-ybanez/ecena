//
// Created by Melvic Ybanez on 12/23/22.
//

#ifndef ECENA_LIGHT_H
#define ECENA_LIGHT_H

#include "../math/include/tuples.h"
#include "materials.h"
#include "../../engine/include/shapes.h"

namespace rt::lights {
    struct PointLight {
        Point position;
        Color intensity;
    };

    Color lighting(
            const Shape& object,
            const Material& material,
            const PointLight& light,
            const Point& point,
            const Vec& eye_vec,
            const Vec& normal_vec,
            bool in_shadow = false
    );

    std::ostream& operator<<(std::ostream& out, const PointLight& light);
}

namespace rt {
    using PointLight = lights::PointLight;
}

#endif //ECENA_LIGHT_H
