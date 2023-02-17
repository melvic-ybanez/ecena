//
// Created by Melvic Ybanez on 12/30/22.
//

#include <cmath>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/world.h"
#include "../include/test_utils.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests::world {
    static void init();

    static void intersections();

    static void shading();

    static void colors();

    static void shadows();

    static void reflections();

    static void refractions();

    void test() {
        set("World", [] {
            init();
            intersections();
            shading();
            colors();
            shadows();
            reflections();
            refractions();
        });
    }

    void init() {
        set("Creating a world", [] {
            World world;
            ASSERT_EQ_MSG("Inspect objects", 0, world.objects.size());
            ASSERT_FALSE_MSG("Inspect light source", world.light.has_value());
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
                auto& shape = world.objects[0];
                Intersection i{4, shape.get()};
                auto comps = comps::prepare(i, ray);
                auto color = world.shade_hit(comps);

                ASSERT_EQ(rt::Color(0.38066, 0.47583, 0.2855), color);
            });
            scenario("Shading an intersection from the inside", [] {
                auto world = default_world();
                world.light = {Point{0, 0.25, 0}, Color::white_};
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                auto& shape = world.objects[1];
                Intersection i{0.5, shape.get()};
                auto comps = comps::prepare(i, ray);
                auto color = world.shade_hit(comps);

                ASSERT_EQ(Color(0.90498, 0.90498, 0.90498), color);
            });
            scenario("An intersection in shadow", [] {
                auto world = default_world();
                world.light = PointLight{{0, 0, -10}, Color::white_};

                std::unique_ptr<Shape> s1 = std::make_unique<shapes::Sphere>();
                std::unique_ptr<Shape> s2 = std::make_unique<shapes::Sphere>();
                math::translate(*s2, 0, 0, 10);
                Intersection i{4, s2.get()};
                world.add_object(s1);
                world.add_object(s2);

                Ray ray{{0, 0, 5},
                        {0, 0, 1}};
                auto comps = comps::prepare(i, ray);
                auto c = world.shade_hit(comps);

                ASSERT_EQ(Color(0.1, 0.1, 0.1), c);
            });
            scenario("Shading with a reflective material", [] {
                auto world = default_world();

                std::unique_ptr<Shape> shape = std::make_unique<shapes::Plane>();
                shape->material->reflectivity = 0.5;
                math::translate(*shape, 0, -1, 0);

                Ray ray{Point{0, 0, -3}, Vec{0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
                Intersection i{std::sqrt(2), shape.get()};
                auto comps = comps::prepare(i, ray);

                world.add_object(shape);

                auto color = world.shade_hit(comps);
                ASSERT_EQ(Color(0.87677, 0.92436, 0.82918), color);
            });
            scenario("Shading with a transparent material", [] {
                auto world = default_world();

                std::unique_ptr<Shape> floor = std::make_unique<shapes::Plane>();
                math::translate(*floor, 0, -1, 0);
                floor->material->transparency = 0.5;
                floor->material->refractive_index = 1.5;

                std::unique_ptr<Shape> ball = std::make_unique<shapes::Sphere>();
                ball->material->color = {1, 0, 0};
                ball->material->ambient = 0.5;
                math::translate(*ball, 0, -3.5, -0.5);

                Ray ray{Point{0, 0, -3}, Vec{0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
                Aggregate xs{{new Intersection{std::sqrt(2), floor.get()}}};
                auto comps = comps::prepare(*xs[0], ray, xs);

                world.add_object(floor);
                world.add_object(ball);

                auto color = world.shade_hit(comps, 5);
                ASSERT_EQ(Color(0.93642, 0.68642, 0.68642), color);
            });
            scenario("With a reflective, transparent material", [] {
                auto world = default_world();

                std::unique_ptr<Shape> floor = std::make_unique<shapes::Plane>();
                math::translate(*floor, 0, -1, 0);
                floor->material->transparency = 0.5;
                floor->material->reflectivity = 0.5;
                floor->material->refractive_index = 1.5;

                std::unique_ptr<Shape> ball = std::make_unique<shapes::Sphere>();
                ball->material->color = {1, 0, 0};
                ball->material->ambient = 0.5;
                math::translate(*ball, 0, -3.5, -0.5);

                Ray ray{Point{0, 0, -3}, Vec{0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
                Aggregate xs{{new Intersection{std::sqrt(2), floor.get()}}};
                auto comps = comps::prepare(*xs[0], ray, xs);

                world.add_object(floor);
                world.add_object(ball);

                auto color = world.shade_hit(comps, 5);
                ASSERT_EQ(Color(0.93391, 0.69643, 0.69243), color);
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

                auto& outer = world.objects[0];
                outer->material->ambient = 1;
                auto& inner = world.objects[1];
                inner->material->ambient = 1;

                Ray ray{Point{0, 0, 0.75}, Vec{0, 0, -1}};
                auto color = world.color_at(ray);
                ASSERT_EQ(inner->material->color, color);
            });
            scenario("Color with mutually reflective surfaces", [] {
                World world;
                world.light = PointLight{{0, 0, 0}, Color::white_};

                std::unique_ptr<Shape> lower = std::make_unique<shapes::Plane>();
                lower->material->reflectivity = 1;
                math::translate(*lower, 0, -1, 0);
                world.add_object(lower);

                std::unique_ptr<Shape> upper = std::make_unique<shapes::Plane>();
                upper->material->reflectivity = 1;
                math::translate(*upper, 0, 1, 0);
                world.add_object(upper);

                Ray ray{{0, 0, 0},
                        {0, 1, 0}};

                // This is just to check that the function terminates successfully.
                // We are not after the actual value here.
                ASSERT_NEQ(Color::black_, world.color_at(ray));
            });
        });
    }

    void shadows() {
        set("Shadows", [] {
            scenario("There is no shadow when nothing is collinear with point and light", [] {
                auto world = default_world();
                Point point{0, 10, 0};
                ASSERT_FALSE(world.is_shadowed_at(point));
            });
            scenario("The shadow when an object is between the point and the light", [] {
                auto world = default_world();
                Point point{10, -10, 10};
                ASSERT_TRUE(world.is_shadowed_at(point));
            });
            scenario("There is no shadow when an object is behind the light", [] {
                auto world = default_world();
                Point point{-20, 20, -10};
                ASSERT_FALSE(world.is_shadowed_at(point));
            });
            scenario("There is no shadow when an object is behind the point", [] {
                auto world = default_world();
                Point point{-2, 2, -2};
                ASSERT_FALSE(world.is_shadowed_at(point));
            });
        });
    }

    void reflections() {
        set("Reflections", [] {
            scenario("The reflected color for a non-reflective material", [] {
                auto world = default_world();
                Ray ray{{0, 0, 0},
                        {0, 0, 1}};

                auto& shape = world.objects[1];
                shape->material->ambient = 1;

                Intersection i{1, shape.get()};
                auto comps = comps::prepare(i, ray);
                auto color = world.reflected_color(comps);
                ASSERT_EQ(Color::black_, color);
            });
            scenario("The reflected color for a reflective material", [] {
                auto world = default_world();

                std::unique_ptr<Shape> shape = std::make_unique<shapes::Plane>();
                shape->material->reflectivity = 0.5;
                math::translate(*shape, 0, -1, 0);

                Ray ray{Point{0, 0, -3},
                        Vec{0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
                Intersection i{std::sqrt(2), shape.get()};
                auto comps = comps::prepare(i, ray);

                world.add_object(shape);

                auto color = world.reflected_color(comps);
                ASSERT_EQ(Color(0.19032, 0.2379, 0.14274), color);
            });
            scenario("The reflected color at the maximum recursive depth", [] {
                auto world = default_world();

                std::unique_ptr<Shape> shape = std::make_unique<shapes::Plane>();
                shape->material->reflectivity = 0.5;
                math::translate(*shape, 0, -1, 0);

                Ray ray{Vec{0, 0, -3},
                        Point{0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
                Intersection i{std::sqrt(2), shape.get()};
                auto comps = comps::prepare(i, ray);

                world.add_object(shape);

                auto color = world.reflected_color(comps, 0);
                ASSERT_EQ(Color::black_, color);
            });
        });
    }

    void refractions() {
        set("Refractions", [] {
            scenario("The refracted color with an opaque surface", [] {
                auto world = default_world();
                auto& shape = world.objects[0];

                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                Aggregate xs{{new Intersection{4, shape.get()}, new Intersection{6, shape.get()}}};
                auto comps = comps::prepare(*xs[0], ray, xs);

                auto color = world.refracted_color(comps, 5);
                ASSERT_EQ(Color::black_, color);
            });
            scenario("The refracted color at the maximum recursive depth", [] {
                auto world = default_world();
                auto& shape = world.objects[0];
                shape->material->transparency = 1.0;
                shape->material->refractive_index = 1.5;

                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                Aggregate xs{{new Intersection{4, shape.get()}, new Intersection{6, shape.get()}}};
                auto comps = comps::prepare(*xs[0], ray, xs);

                auto color = world.reflected_color(comps, 0);
                ASSERT_EQ(Color::black_, color);
            });
            scenario("The refracted color under total internal reflection", [] {
                auto world = default_world();
                auto& shape = world.objects[0];
                shape->material->transparency = 1.0;
                shape->material->refractive_index = 1.5;

                Ray ray{Point{0, 0, std::sqrt(2) / 2}, Vec{0, 1, 0}};
                Aggregate xs{{new Intersection{-std::sqrt(2) / 2, shape.get()},
                              new Intersection{std::sqrt(2) / 2, shape.get()}}};

                auto comps = comps::prepare(*xs[1], ray, xs);
                auto color = world.reflected_color(comps, 5);
                ASSERT_EQ(Color::black_, color);
            });
            scenario("The refracted color with a refracted ray", [] {
                auto world = default_world();

                auto& shape1 = world.objects[0];
                shape1->material->ambient = 1.0;
                shape1->material->pattern = std::make_unique<TestPattern>();

                auto& shape2 = world.objects[1];
                shape2->material->transparency = 1.0;
                shape2->material->refractive_index = 1.5;

                Ray ray{Point{0, 0, 0.1}, Vec{0, 1, 0}};
                Aggregate xs{{new Intersection{-0.9899, shape1.get()}, new Intersection{-0.4899, shape2.get()},
                              new Intersection{0.4899, shape2.get()}, new Intersection{0.9899, shape1.get()}}};
                auto comps = comps::prepare(*xs[2], ray, xs);
                auto color = world.refracted_color(comps, 5);

                ASSERT_EQ(Color(0, 0.99888, 0.04725), color);
            });
        });
    }
}