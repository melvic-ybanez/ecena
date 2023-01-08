//
// Created by Melvic Ybanez on 12/31/22.
//

#include <cmath>
#include <ostream>
#include "../include/world.h"
#include "../../shared/include/utils.h"

namespace rt {
    Aggregate World::intersect(const Ray &ray) const {
        Aggregate aggregate;

        for (auto &obj: objects) {
            auto agg = obj->intersect(ray);
            aggregate.combine_with(agg);
        }
        return aggregate;
    }

    Color World::shade_hit(const Comps &comps, int remaining) const {
        if (!light.has_value()) return Color::black_;
        auto shadowed = is_shadowed_at(comps.over_point);
        auto surface = lights::lighting(*comps.object, *comps.object->material, light.value(), comps.point, comps.eye_vec,
                                comps.normal_vec, shadowed);

        auto reflected = reflected_color(comps, remaining);
        auto refracted = refracted_color(comps, remaining);

        auto &mat = comps.object->material;
        if (mat->is_reflective() && mat->is_transparent()) {
            auto reflectance = schlick(comps);
            return surface + reflected * reflectance + refracted * (1 - reflectance);
        }

        return surface + reflected + refracted;
    }

    Color World::color_at(const Ray &ray, int remaining) const {
        auto xs = intersect(ray);
        auto hit = xs.hit();
        if (hit == nullptr) return Color::black_;
        auto comps = comps::prepare(*hit, ray, xs);
        return shade_hit(comps, remaining);
    }

    std::ostream &operator<<(std::ostream &out, const World &world) {
        return out << "{ objects: " << join_to_array(world.objects)
                   << ", light: " << optional_to_str(world.light)
                   << " }";
    }

    bool World::is_shadowed_at(const Point &point) const {
        auto direction = Vec(light->position - point);
        auto distance = direction.magnitude();
        Ray ray{point, direction.normalize()};
        auto xs = intersect(ray);
        auto hit = xs.hit();

        return hit != nullptr && hit->t < distance;
    }

    World &World::add_object(std::unique_ptr<Shape> &shape) {
        objects.push_back(std::move(shape));
        return *this;
    }

    Color World::reflected_color(const Comps &comps, int remaining) const {
        if (remaining == 0 || !comps.object->material->is_reflective()) return Color::black_;

        Ray reflect_ray{comps.over_point, comps.reflect_vec};
        auto color = color_at(reflect_ray, remaining - 1);

        return color * comps.object->material->reflectivity;
    }

    Color World::refracted_color(const Comps &comps, int remaining) const {
        if (remaining == 0 || !comps.object->material->is_transparent()) return Color::black_;

        auto n_ratio = 0.0;
        auto cos_i = 0.0;
        auto sin2_t = 0.0;

        // This is based on Snell's Law:
        // sin(a) / sin(b) = n2 / n1, given two angles a and b
        auto check_total_internal_reflection = [&] {
            n_ratio = comps.n1 / comps.n2;
            cos_i = comps.eye_vec.dot(comps.normal_vec);
            // Pythagorean identity: sin^2 (t) + cos^2 (t) = 1
            sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
            return sin2_t > 1;
        };

        if (check_total_internal_reflection()) return Color::black_;

        // again, from Pythagorean identities
        auto cos_t = std::sqrt(1.0 - sin2_t);

        // the direction of the refracted ray
        auto direction = comps.normal_vec * (n_ratio * cos_i - cos_t) - comps.eye_vec * n_ratio;

        Ray refracted_ray{comps.under_point, direction};

        // color of the refracted ray, with opacity (if any)
        auto color = color_at(refracted_ray, remaining - 1) * comps.object->material->transparency;

        return color;
    }
}