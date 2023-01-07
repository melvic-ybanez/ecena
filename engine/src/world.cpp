//
// Created by Melvic Ybanez on 12/31/22.
//

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
        return surface + reflected;
    }

    Color World::color_at(const Ray &ray, int remaining) const {
        auto xs = intersect(ray);
        auto hit = xs.hit();
        if (hit == nullptr) return Color::black_;
        Comps comps{*hit, ray};
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
        if (remaining < 1) return Color::black_;

        if (!comps.object->material->is_reflective()) return Color::black_;

        Ray reflect_ray{comps.over_point, comps.reflect_vec};
        auto color = color_at(reflect_ray, remaining - 1);

        return color * comps.object->material->reflectivity;
    }
}