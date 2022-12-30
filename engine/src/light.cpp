//
// Created by Melvic Ybanez on 12/24/22.
//

#include <cmath>
#include "../include/light.h"

namespace rt::lights {
    Color lighting(
            const Material &material,
            const PointLight &light,
            const Point &point,
            const Vec &eye_vec,
            const Vec &normal_vec
    ) {
        auto effective_color = material.color * light.intensity;
        auto light_vec = Vec(light.position - point).normalize();
        auto ambient = effective_color * material.ambient;
        auto diffuse = Color::black_;
        auto specular = Color::black_;

        auto light_dot_normal = light_vec.dot(normal_vec);

        // if the light is not the other side of the surface...
        if (light_dot_normal >= 0) {
            diffuse = effective_color * material.diffuse * light_dot_normal;
            auto reflect_vec = Vec(-light_vec).reflect(normal_vec);
            auto reflect_dot_eye = reflect_vec.dot(eye_vec);

            // if the light does not reflect away from the eye...
            if (reflect_dot_eye > 0) {
                auto factor = std::pow(reflect_dot_eye, material.shininess);
                specular = light.intensity * material.specular * factor;
            }
        }

        return {ambient + diffuse + specular};
    }

    std::ostream &operator<<(std::ostream &out, const PointLight &light) {
        return out << "{ position: " << light.position << ", intensity: " << light.intensity << " }";
    }
}