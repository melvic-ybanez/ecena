//
// Created by Melvic Ybanez on 12/19/22.
//

#include <map>
#include <cmath>
#include <array>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/intersection.h"
#include "../../engine/include/shapes.h"
#include "../../engine/include/comps.h"
#include "../../engine/math/include/transform.h"
#include "../include/test_utils.h"

namespace rt::tests::intersections {
    static void init();

    static void aggregations();

    static void hits();

    static void computations();

    static void reflections();

    static void refractions();

    static void under_point();

    void test() {
        set("Intersections", [] {
            init();
            aggregations();
            hits();
            computations();
            reflections();
            refractions();
            under_point();
        });
    }

    void init() {
        scenario("An intersection encapsulates t and to_data", [] {
            auto sphere = new shapes::Sphere;
            Intersection i{3.5, sphere};
            ASSERT_EQ_MSG("Inspect t", 3.5, i.t);
            ASSERT_EQ_MSG("Inspect to_data", shapes::Type::sphere, sphere->type());
        });
    }

    void aggregations() {
        set("Aggregating intersections", [] {
            shapes::Sphere sphere;
            auto i1 = new Intersection{1, &sphere};
            auto i2 = new Intersection{2, &sphere};
            Aggregate agg{{i1, i2}};

            ASSERT_EQ_MSG("Inspect count", 2, agg.count());
            ASSERT_EQ_MSG("Inspect first element", i1, agg[0]);
            ASSERT_EQ_MSG("Inspect second element", i2, agg[1]);
        });
        set("Intersect sets the to_data on the intersection", [] {
            Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
            shapes::Sphere sphere;
            auto agg = sphere.intersect(ray);

            ASSERT_EQ_MSG("Inspect count", 2, agg.count());
            ASSERT_EQ_MSG("Inspect first to_data", &sphere, agg[0]->object);
            ASSERT_EQ_MSG("Inspect second to_data", &sphere, agg[1]->object);
        });
    }

    void hits() {
        set("Hit", [] {
            shapes::Sphere sphere;

            scenario("When all intersections have positive ts", [&] {
                auto i1 = new Intersection{1, &sphere};
                auto i2 = new Intersection{2, &sphere};
                Aggregate agg{{i2, i1}};
                ASSERT_EQ(i1, agg.hit());
            });
            scenario("When some intersections have negative ts", [&] {
                auto i1 = new Intersection{-1, &sphere};
                auto i2 = new Intersection{2, &sphere};
                Aggregate agg{{i2, i1}};
                ASSERT_EQ(i2, agg.hit());
            });
            scenario("When all intersections have negative ts", [&] {
                auto i1 = new Intersection{-2, &sphere};
                auto i2 = new Intersection{-1, &sphere};
                Aggregate agg{{i2, i1}};
                ASSERT_EQ(nullptr, agg.hit());
            });
            scenario("Always the lowest non-negative intersection", [&] {
                auto i1 = new Intersection{5, &sphere};
                auto i2 = new Intersection{7, &sphere};
                auto i3 = new Intersection{-3, &sphere};
                auto i4 = new Intersection{2, &sphere};
                Aggregate agg{{i1, i2, i3, i4}};
                ASSERT_EQ(i4, agg.hit());
            });

            set("The hit should offset the point", [] {
                Ray ray{{0, 0, -5},
                        {0, 0, 1}};
                shapes::Sphere sphere;
                math::translate(sphere, 0, 0, 1);
                Intersection i{5, &sphere};
                auto comps = comps::prepare(i, ray);

                ASSERT_TRUE_MSG("Over point z", comps.over_point.z() < -math::epsilon / 2);
                ASSERT_TRUE_MSG("Point z", comps.point.z() > comps.over_point.z());
            });
        });
    }

    void computations() {
        set("Computations", [] {
            shapes::Sphere shape;

            set("Precomputing the state of an intersection", [&] {
                Ray ray{{0, 0, -5},
                        {0, 0, 1}};
                Intersection i{4, &shape};
                auto comps = comps::prepare(i, ray);

                ASSERT_EQ_MSG("t", i.t, comps.t);
                ASSERT_EQ_MSG("object", i.object, comps.object);
                ASSERT_EQ_MSG("point", Point(0, 0, -1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_EQ_MSG("normal vector", Vec(0, 0, -1), comps.normal_vec);
            });
            scenario("The hit, when an intersection occurs on the outside", [&] {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                Intersection i{4, &shape};
                auto comps = comps::prepare(i, ray);

                ASSERT_FALSE(comps.inside);
            });
            set("The hit, when an intersection occurs on the inside", [&] {
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                Intersection i{1, &shape};
                auto comps = comps::prepare(i, ray);

                ASSERT_EQ_MSG("point", Point(0, 0, 1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_TRUE_MSG("inside", comps.inside);
                ASSERT_EQ_MSG("normal", Vec(0, 0, -1), comps.normal_vec);   // normal is inverted
            });
        });
    }

    void reflections() {
        scenario("Precomputing the reflection vector", [] {
            shapes::Plane shape;
            Ray ray{{0, 1,                 -1},
                    {0, -std::sqrt(2) / 2, std::sqrt(2) / 2}};
            Intersection i{std::sqrt(2), &shape};
            auto comps = comps::prepare(i, ray);
            ASSERT_EQ(Vec(0, std::sqrt(2) / 2, std::sqrt(2) / 2), comps.reflect_vec);
        });
    }

    void refractions() {
        set("Finding n1 and n2 at various intersections", [] {
            auto s1 = glass_sphere();
            math::scale(*s1, 2, 2, 2);
            s1->material->refractive_index = 1.5;

            auto s2 = glass_sphere();
            math::translate(*s2, 0, 0, -0.25);
            s2->material->refractive_index = 2.0;

            auto s3 = glass_sphere();
            math::translate(*s3, 0, 0, 0.25);
            s3->material->refractive_index = 2.5;

            Ray ray{Point{0, 0, -4}, Vec{0, 0, 1}};
            Aggregate xs{{new Intersection{2, s1.get()}, new Intersection{2.75, s2.get()},
                          new Intersection{3.25, s3.get()}, new Intersection{4.75, s2.get()},
                          new Intersection{5.25, s3.get()}, new Intersection{6, s1.get()}}};

            std::map<std::string, std::vector<real>> data{
                    {"index", {0,   1,   2,   3,   4,   5}},
                    {"n1",    {1.0, 1.5, 2.0, 2.5, 2.5, 1.5}},
                    {"n2",    {1.5, 2.0, 2.5, 2.5, 1.5, 1.0}}
            };

            for (int i = 0; i < data["index"].size(); i++) {
                auto index = static_cast<size_t>(data["index"][i]);
                auto n1 = data["n1"][i];
                auto n2 = data["n2"][i];
                auto comps = comps::prepare(*xs[index], ray, xs);
                auto index_label = "{index: " + std::to_string(index);

                ASSERT_EQ_MSG(index_label + ", n1: " + std::to_string(n1) + "}", n1, comps.n1);
                ASSERT_EQ_MSG(index_label + ", n2: " + std::to_string(n2) + "}", n2, comps.n2);
            }
        });
    }

    void under_point() {
        set("The under point is offset below the surface", [] {
            Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
            auto shape = glass_sphere();
            math::translate(*shape, 0, 0, 1);
            auto i = new Intersection{5, shape.get()};
            Aggregate xs{{i}};
            auto comps = comps::prepare(*i, ray, xs);

            ASSERT_TRUE_MSG("Under point", comps.under_point.z() > math::epsilon / 2);
            ASSERT_TRUE_MSG("Point z-axis", comps.point.z() < comps.under_point.z());
        });
    }
}