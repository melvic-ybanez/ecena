//
// Created by Melvic Ybanez on 12/23/22.
//

#include "../include/material.h"

namespace rt::materials {
    Material::Material() : color{Color::white_}, ambient{0.1}, diffuse{0.9}, specular{0.9}, shininess{200} {}
}