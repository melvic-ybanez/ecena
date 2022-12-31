//
// Created by Melvic Ybanez on 12/31/22.
//

#include "../include/world.h"

namespace rt {
    World::~World() {
        for (auto &e: objects) {
            delete e;
            e = nullptr;
        }
        objects.clear();
    }

    intersections::Aggregate World::intersect(const Ray &ray) const {
        intersections::Aggregate aggregate;
        for (auto obj: objects) {
            auto agg = obj->intersect(ray);
            aggregate.combine_with(agg);
        }
        return aggregate;
    }

    Color World::shade_hit(const Comps &comps) const {
        if (!light.has_value()) return Color::black_;
        return lights::lighting(comps.object->material, light.value(), comps.point, comps.eye_vec, comps.normal_vec);
    }

    Shape *World::operator[](size_t i) {
        return objects[i];
    }
}