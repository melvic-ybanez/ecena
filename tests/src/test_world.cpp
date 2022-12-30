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

            intersections();
        });
    }

    void init() {
        set("Creating a world", [] {
            rt::World world;
            ASSERT_EQ_MSG("Inspect objects", 0, world.objects.size());
            ASSERT_TRUE_MSG("Inspect light source", !world.light.has_value());
        });
    }

    void intersections() {
        set("Intersections", [] {
            auto world = default_world();
            rt::Ray ray{rt::Point{0, 0, -5}, rt::Vec{0, 0, 1}};
            auto xs = world.intersect(ray);

            ASSERT_EQ_MSG("Count", 4, xs.count());
            ASSERT_EQ_MSG("1st intersection", 4, xs[0]->t);
            ASSERT_EQ_MSG("2nd intersection", 4.5, xs[1]->t);
            ASSERT_EQ_MSG("3rd intersection", 5.5, xs[2]->t);
            ASSERT_EQ_MSG("4th intersection", 6, xs[3]->t);
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

        return std::move(opt_world.value());
    }
}