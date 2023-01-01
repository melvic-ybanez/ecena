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

    Color World::shade_hit(const Comps &comps) const {
        if (!light.has_value()) return Color::black_;
        return lights::lighting(comps.object->material, light.value(), comps.point, comps.eye_vec, comps.normal_vec);
    }

    Color World::color_at(const Ray &ray) const {
        auto xs = intersect(ray);
        auto hit = xs.hit();
        if (hit == nullptr) return Color::black_;
        Comps comps{*hit, ray};
        return shade_hit(comps);
    }

    std::ostream &operator<<(std::ostream &out, const World &world) {
        return out << "{ objects: " << join_to_array(world.objects)
                   << ", light: " << optional_to_str(world.light)
                   << " }";
    }
}