//
// Created by Melvic Ybanez on 1/1/23.
//

#include "../include/test_utils.h"

namespace rt::tests {
    rt::World default_world() {
        rt::World world;
        world.light = PointLight{{-10, 10, -10}, Color::white_};

        auto s1 = std::make_unique<shapes::Sphere>();
        s1->material.color = {0.8, 1.0, 0.6};
        s1->material.diffuse = 0.7;
        s1->material.specular = 0.2;

        auto s2 = std::make_unique<shapes::Sphere>();
        s2->transformation = matrix::scaling(0.5, 0.5, 0.5);

        world.objects.push_back(std::move(s1));
        world.objects.push_back(std::move(s2));

        return world;
    }
}