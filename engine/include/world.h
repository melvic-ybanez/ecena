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

        Aggregate intersect(const Ray &ray) const;

        Color shade_hit(const Comps &comps, int remaining = default_depth) const;

        Color color_at(const Ray &ray, int remaining = default_depth) const;

        bool is_shadowed_at(const Point &point) const;

        World &add_object(std::unique_ptr<Shape> &shape);

        Color reflected_color(const Comps &comps, int remaining = default_depth) const;

    private:
        static inline int default_depth = 4;
    };

    std::ostream &operator<<(std::ostream &out, const World &world);
}

#endif //ECENA_WORLD_H
