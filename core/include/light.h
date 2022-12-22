//
// Created by Melvic Ybanez on 12/23/22.
//

#ifndef ECENA_LIGHT_H
#define ECENA_LIGHT_H

#include "../../math/include/tuples.h"

namespace rt::lights {
    struct PointLight {
        Point position;
        Color intensity;
    };
}

namespace rt {
    using PointLight = lights::PointLight;
}

#endif //ECENA_LIGHT_H
