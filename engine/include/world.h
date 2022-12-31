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
        std::vector<Shape *> objects;
        std::optional<PointLight> light;

        intersections::Aggregate intersect(const Ray &ray) const;

        World() = default;

        World(const World &from) = default;

        World(World &&from) noexcept = default;

        World &operator=(const World &from) = default;

        World &operator=(World &&from) noexcept = default;

        ~World();

        Color shade_hit(const Comps &comps) const;

        Color color_at(const Ray &ray) const;
    };
}

#endif //ECENA_WORLD_H
