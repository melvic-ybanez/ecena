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
        real transparency;
        real refractive_index;

        Material();

        bool operator==(const Material& other) const;

        bool operator!=(const Material& other) const;

        Color color_at(const Shape& object, const Point& point) const;

        bool is_reflective() const;

        bool is_transparent() const;
    };

    Material glass();

    std::ostream& operator<<(std::ostream& out, const Material& mat);
}

namespace rt::materials::refractive_indices {
    constexpr inline real vacuum = 1;
    constexpr inline real air = 1.00029;
    constexpr inline real water = 1.333;
    constexpr inline real glass = 1.5;
    constexpr inline real diamond = 2.417;
}

namespace rt {
    using Material = materials::Material;
}

#endif //ECENA_MATERIALS_H
