//
// Created by Melvic Ybanez on 12/23/22.
//

#ifndef ECENA_MATERIALS_H
#define ECENA_MATERIALS_H

#include "../math/include/math_utils.h"
#include "../math/include/tuples.h"
#include "patterns.h"

namespace rt::materials {
    class Material {
    public:
        Color color;
        real ambient;
        real diffuse;
        real specular;
        real shininess;
        std::unique_ptr<Pattern> pattern;
        real reflectivity;

        Material();

        bool operator==(const Material &other) const;

        bool operator!=(const Material &other) const;

        Color color_at(const Shape &object, const Point &point) const;

        bool is_reflective() const;
    };

    std::ostream &operator<<(std::ostream &out, const Material &mat);
}

namespace rt {
    using Material = materials::Material;
}

#endif //ECENA_MATERIALS_H
