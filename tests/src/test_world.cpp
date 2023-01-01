//
// Created by Melvic Ybanez on 12/30/22.
//

#include "../include/test_world.h"
#include "../include/asserts.h"
#include "../../engine/include/world.h"
#include "../include/test_utils.h"

namespace rt::tests::world {
    void all() {
        set("World", [] {
            init();
            intersections();
            shading();
            colors();
        });
    }

    void init() {
        set("Creating a world", [] {
            World world;
            ASSERT_EQ_MSG("Inspect objects", 0, world.objects.size());
            ASSERT_TRUE_MSG("Inspect light source", !world.light.has_value());
        });
    }

    void intersections() {
        set("Intersections", [] {
            auto world = default_world();
            Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
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
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                auto &shape = world.objects[0];
                Intersection i{4, shape.get()};
                Comps comps{i, ray};
                auto color = world.shade_hit(comps);

                ASSERT_EQ(rt::Color(0.38066, 0.47583, 0.2855), color);
            });
            scenario("Shading an intersection from the inside", [] {
                auto world = default_world();
                world.light = {Point{0, 0.25, 0}, Color::white_};
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                auto &shape = world.objects[1];
                Intersection i{0.5, shape.get()};
                Comps comps{i, ray};
                auto color = world.shade_hit(comps);

                ASSERT_EQ(Color(0.90498, 0.90498, 0.90498), color);
            });
        });
    }

    void colors() {
        set("Colors", [] {
            scenario("When a ray misses", [] {
                auto world = default_world();
                Ray ray{Point{0, 0, -5}, Vec{0, 1, 0}};
                auto color = world.color_at(ray);
                ASSERT_EQ(Color::black_, color);
            });
            scenario("When a ray hits", [] {
                auto world = default_world();
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                auto color = world.color_at(ray);
                ASSERT_EQ(Color(0.38066, 0.47583, 0.2855), color);
            });
            scenario("An intersection behind the ray", [] {
                auto world = default_world();
                auto &outer = world.objects[0];
                outer->material.ambient = 1;
                auto &inner = world.objects[1];
                inner->material.ambient = 1;
                Ray ray{Point{0, 0, 0.75}, Vec{0, 0, -1}};
                auto color = world.color_at(ray);
                ASSERT_EQ(inner->material.color, color);
            });
        });
    }
}