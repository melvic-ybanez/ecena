//
// Created by Melvic Ybanez on 12/23/22.
//

#include <ostream>
#include "../include/materials.h"

namespace rt::materials {
    Material::Material() : color{Color::white_}, ambient{0.1}, diffuse{0.9}, specular{0.9}, shininess{200},
                           pattern{nullptr}, reflectivity{0}, transparency{0}, refractive_index{1.0} {}

    bool Material::operator==(const Material &other) const {
        return this->color == other.color
               && this->ambient == other.ambient
               && this->diffuse == other.diffuse
               && this->specular == other.specular
               && this->shininess == other.shininess;
    }

    bool Material::operator!=(const Material &other) const {
        return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &out, const Material &mat) {
        return out << "Material { " << mat.color << ", " << mat.ambient << ", " << mat.diffuse << ", " << mat.specular
                   << ", " << mat.shininess << ", transparency: " << mat.transparency << ", reflectivity: "
                   << mat.reflectivity << ", refractive_index: " << mat.refractive_index << " }";
    }

    Color Material::color_at(const Shape &object, const Point &point) const {
        if (pattern == nullptr) return color;
        return pattern->at(object, point);
    }

    bool Material::is_reflective() const {
        return reflectivity > 0;
    }

    bool Material::is_transparent() const {
        return transparency > 0;
    }
}