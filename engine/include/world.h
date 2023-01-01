//
// Created by Melvic Ybanez on 12/30/22.
//

#ifndef ECENA_WORLD_H
#define ECENA_WORLD_H

#include <vector>
#include <optional>
#include "shapes.h"
#include "light.h"
#include "comps.h"

namespace rt {
    class World {
    public:
        std::vector<std::unique_ptr<Shape>> objects;
        std::optional<PointLight> light;

        intersections::Aggregate intersect(const Ray &ray) const;

        Color shade_hit(const Comps &comps) const;

        Color color_at(const Ray &ray) const;
    };

    std::ostream &operator<<(std::ostream &out, const World &world);
}

#endif //ECENA_WORLD_H
