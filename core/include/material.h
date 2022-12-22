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

        bool operator==(const Material &other) const;

        bool operator!=(const Material &other) const;
    };

    std::ostream &operator<<(std::ostream &out, const Material &mat);
}

namespace rt {
    using Material = materials::Material;
}

#endif //ECENA_MATERIAL_H
