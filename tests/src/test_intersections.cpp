//
// Created by Melvic Ybanez on 12/19/22.
//

#include <array>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/intersection.h"
#include "../../engine/include/shapes.h"
#include "../../engine/include/comps.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests::intersections {
    static void init();

    static void aggregations();

    static void hits();

    static void computations();

    void test() {
        set("Intersections", [] {
            init();
            aggregations();
            hits();
            computations();
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
                Comps comps{i, ray};

                ASSERT_TRUE_MSG("Over point z", comps.over_point.z() < -math::epsilon / 2);
                ASSERT_TRUE_MSG("Point z", comps.point.z() > comps.over_point.z());
            });
        });
    }

    void computations() {
        set("Computations", [] {
            shapes::Sphere shape;

            set("Precomputing the state of an intersection", [&] {
                Ray ray{{0, 0, -5}, {0, 0, 1}};
                Intersection i{4, &shape};
                Comps comps{i, ray};

                ASSERT_EQ_MSG("t", i.t, comps.t);
                ASSERT_EQ_MSG("object", i.object, comps.object);
                ASSERT_EQ_MSG("point", Point(0, 0, -1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_EQ_MSG("normal vector", Vec(0, 0, -1), comps.normal_vec);
            });
            scenario("The hit, when an intersection occurs on the outside", [&] {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                Intersection i{4, &shape};
                Comps comps{i, ray};

                ASSERT_FALSE(comps.inside);
            });
            set("The hit, when an intersection occurs on the inside", [&] {
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                Intersection i{1, &shape};
                Comps comps{i, ray};

                ASSERT_EQ_MSG("point", Point(0, 0, 1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_TRUE_MSG("inside", comps.inside);
                ASSERT_EQ_MSG("normal", Vec(0, 0, -1), comps.normal_vec);   // normal is inverted
            });
        });
    }
}