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
        constexpr static int default_depth_ = 5;

        std::vector<std::unique_ptr<Shape>> objects;
        std::optional<PointLight> light;    // the source of light

        Aggregate intersect(const Ray &ray) const;

        Color shade_hit(const Comps &comps, int remaining = default_depth_) const;

        Color color_at(const Ray &ray, int remaining = default_depth_, const Color &bg_color = Color::black_) const;

        bool is_shadowed_at(const Point &point) const;

        World &add_object(std::unique_ptr<Shape> &shape);

        Color reflected_color(const Comps &comps, int remaining = default_depth_) const;

        Color refracted_color(const Comps &comps, int remaining = default_depth_) const;
    };

    std::ostream &operator<<(std::ostream &out, const World &world);
}

#endif //ECENA_WORLD_H
