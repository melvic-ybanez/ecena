//
// Created by Melvic Ybanez on 12/23/22.
//

#ifndef ECENA_MATERIAL_H
#define ECENA_MATERIAL_H

#include "../../math/include/math_utils.h"
#include "../../math/include/tuples.h"

namespace rt::materials {
    struct Material {
        Color color;
        real ambient;
        real diffuse;
        real specular;
        real shininess;

        Material();
    };
}

namespace rt {
    using Material = materials::Material;
}

#endif //ECENA_MATERIAL_H
