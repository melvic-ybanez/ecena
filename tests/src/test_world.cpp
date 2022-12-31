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
            shading();
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

    void shading() {
        set("Shading", [] {
            scenario("Shading an intersection", [] {
                auto world = default_world();
                rt::Ray ray{rt::Point{0, 0, -5}, rt::Vec{0, 0, 1}};
                auto shape = world[0];
                rt::Intersection i{4, shape};
                rt::Comps comps{i, ray};
                auto color = world.shade_hit(comps);

                ASSERT_EQ(rt::Color(0.38066, 0.47583, 0.2855), color);
            });
            scenario("Shading an intersection from the inside", [] {
                auto world = default_world();
                world.light = {rt::Point{0, 0.25, 0}, rt::Color::white_};
                rt::Ray ray{rt::Point{0, 0, 0}, rt::Vec{0, 0, 1}};
                auto shape = world[1];
                rt::Intersection i{0.5, shape};
                rt::Comps comps{i, ray};
                auto color = world.shade_hit(comps);

                ASSERT_EQ(rt::Color(0.90498, 0.90498, 0.90498), color);
            });
        });
    }

    rt::World default_world() {
        rt::World world;
        world.light = rt::PointLight{{-10, 10, -10}, rt::Color::white_};

        auto s1 = new rt::shapes::Sphere;
        s1->material.color = {0.8, 1.0, 0.6};
        s1->material.diffuse = 0.7;
        s1->material.specular = 0.2;

        auto s2 = new rt::shapes::Sphere;
        s2->transformation = rt::math::matrix::scaling(0.5, 0.5, 0.5);

        world.objects.push_back(s1);
        world.objects.push_back(s2);

        return world;
    }
}