//
// Created by Melvic Ybanez on 12/19/22.
//

#include <array>
#include "../include/test_intersections.h"
#include "../include/asserts.h"
#include "../../engine/include/intersection.h"
#include "../../engine/include/shapes.h"
#include "../../engine/include/comps.h"

namespace rt::tests::intersections {
    void all() {
        set("Intersections", [] {
            init();
            aggregations();
            hits();
            computations();
        });
    }

    void init() {
        scenario("An intersection encapsulates t and to_data", [] {
            auto sphere = new rt::shapes::Sphere;
            rt::Intersection i{3.5, sphere};
            ASSERT_EQ_MSG("Inspect t", 3.5, i.t);
            ASSERT_EQ_MSG("Inspect to_data", rt::shapes::Type::sphere, sphere->type());
        });
    }

    void aggregations() {
        set("Aggregating intersections", [] {
            rt::shapes::Sphere sphere;
            rt::Intersection i1{1, &sphere};
            rt::Intersection i2{2, &sphere};
            rt::intersections::Aggregate agg{{&i1, &i2}};

            ASSERT_EQ_MSG("Inspect count", 2, agg.count());
            ASSERT_EQ_MSG("Inspect first element", &i1, agg[0]);
            ASSERT_EQ_MSG("Inspect second element", &i2, agg[1]);
        });
        set("Intersect sets the to_data on the intersection", [] {
            Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
            rt::shapes::Sphere sphere;
            auto agg = sphere.intersect(ray);

            ASSERT_EQ_MSG("Inspect count", 2, agg.count());
            ASSERT_EQ_MSG("Inspect first to_data", &sphere, agg[0]->object);
            ASSERT_EQ_MSG("Inspect second to_data", &sphere, agg[1]->object);
        });
    }

    void hits() {
        set("Hit", [] {
            rt::shapes::Sphere sphere;

            scenario("When all intersections have positive ts", [&] {
                rt::Intersection i1{1, &sphere};
                rt::Intersection i2{2, &sphere};
                rt::intersections::Aggregate agg{{&i2, &i1}};
                ASSERT_EQ(&i1, agg.hit());
            });
            scenario("When some intersections have negative ts", [&] {
                rt::Intersection i1{-1, &sphere};
                rt::Intersection i2{2, &sphere};
                rt::intersections::Aggregate agg{{&i2, &i1}};
                ASSERT_EQ(&i2, agg.hit());
            });
            scenario("When all intersections have negative ts", [&] {
                rt::Intersection i1{-2, &sphere};
                rt::Intersection i2{-1, &sphere};
                rt::intersections::Aggregate agg{{&i2, &i1}};
                ASSERT_EQ(nullptr, agg.hit());
            });
            scenario("Always the lowest non-negative intersection", [&] {
                rt::Intersection i1{5, &sphere};
                rt::Intersection i2{7, &sphere};
                rt::Intersection i3{-3, &sphere};
                rt::Intersection i4{2, &sphere};
                rt::intersections::Aggregate agg{{&i1, &i2, &i3, &i4}};
                ASSERT_EQ(&i4, agg.hit());
            });
        });
    }

    void computations() {
        set("Computations", [] {
            rt::shapes::Sphere shape;

            set("Precomputing the state of an intersection", [&] {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                rt::Intersection i{4, &shape};
                rt::Comps comps{i, ray};

                ASSERT_EQ_MSG("t", i.t, comps.t);
                ASSERT_EQ_MSG("object", i.object, comps.object);
                ASSERT_EQ_MSG("point", Point(0, 0, -1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_EQ_MSG("normal vector", Vec(0, 0, -1), comps.normal_vec);
            });
            scenario("The hit, when an intersection occurs on the outside", [&] {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                rt::Intersection i{4, &shape};
                rt::Comps comps{i, ray};

                ASSERT_TRUE(!comps.inside);
            });
            set("The hit, when an intersection occurs on the inside", [&] {
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                rt::Intersection i{1, &shape};
                rt::Comps comps{i, ray};

                ASSERT_EQ_MSG("point", Point(0, 0, 1), comps.point);
                ASSERT_EQ_MSG("eye vector", Vec(0, 0, -1), comps.eye_vec);
                ASSERT_TRUE_MSG("inside", comps.inside);
                ASSERT_EQ_MSG("normal", Vec(0, 0, -1), comps.normal_vec);   // normal is inverted
            });
        });
    }
}