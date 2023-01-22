//
// Created by Melvic Ybanez on 1/1/23.
//

#include "../include/test_utils.h"

namespace rt::tests {
    rt::World default_world() {
        rt::World world;
        world.light = PointLight{{-10, 10, -10}, Color::white_};

        auto s1 = std::make_unique<shapes::Sphere>();
        s1->material->color = {0.8, 1.0, 0.6};
        s1->material->diffuse = 0.7;
        s1->material->specular = 0.2;

        auto s2 = std::make_unique<shapes::Sphere>();
        s2->transformation = matrix::scaling(0.5, 0.5, 0.5);

        world.objects.push_back(std::move(s1));
        world.objects.push_back(std::move(s2));

        return world;
    }

    shapes::Type TestShape::type() const {
        return shapes::Type::test;
    }

    Aggregate TestShape::local_intersect(const Ray &local_ray) {
        this->saved_ray = local_ray;
        return {};
    }

    Vec TestShape::local_normal_at(const Point &local_point) {
        return {local_point.x(), local_point.y(), local_point.z()};
    }

    Bounds TestShape::bounds() const {
        return Bounds::cube();
    }

    Color TestPattern::at(const Point &point) const {
        return point;
    }

    std::unique_ptr<Shape> glass_sphere() {
        std::unique_ptr<Shape> sphere = std::make_unique<shapes::Sphere>();
        sphere->material->transparency = 1.0;
        sphere->material->refractive_index = materials::refractive_indices::glass;
        return sphere;
    }
}