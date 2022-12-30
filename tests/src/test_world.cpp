//
// Created by Melvic Ybanez on 12/30/22.
//

#include <optional>
#include "../include/test_world.h"
#include "../include/asserts.h"
#include "../../engine/include/world.h"

namespace tests::world {
    static rt::World default_world();

    void all() {
        set("World", [] {
            init();

            auto world = default_world();
            for (auto object: world.objects) {
                delete object;
            }
        });
    }

    void init() {
        set("Creating a world", [] {
           rt::World world;
            ASSERT_EQ_MSG("Inspect objects", 0, world.objects.size());
            ASSERT_TRUE_MSG("Inspect light source", !world.light.has_value());
        });
    }

    rt::World default_world() {
        static std::optional<rt::World> opt_world;

        if (opt_world.has_value()) return opt_world.value();
        opt_world = {{}};

        auto &world = opt_world.value();
        world.light = rt::PointLight{{-10, 10, -10}, rt::Color::white_};

        auto s1 = new rt::shapes::Sphere;
        s1->material.color = {0.8, 1.0, 0.6};
        s1->material.diffuse = 0.7;
        s1->material.specular = 0.2;

        auto s2 = new rt::shapes::Sphere;
        s2->transformation = rt::math::matrix::scaling(0.5, 0.5, 0.5);

        world.objects.push_back(s1);
        world.objects.push_back(s2);

        return opt_world.value();
    }
}