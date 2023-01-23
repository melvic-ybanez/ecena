//
// Created by Melvic Ybanez on 1/23/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/bounds.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::bounds {
    static void init();

    static void addition();

    static void of_shapes();

    void test() {
        set("Bounds", [] {
            init();
            addition();
            of_shapes();
        });
    }

    void init() {
        set("Create an empty bounding box", [] {
            Bounds box;
            ASSERT_EQ_MSG("Inspect min", Point(math::infinity, math::infinity, math::infinity), box.min);
            ASSERT_EQ_MSG("Inspect max", Point(-math::infinity, -math::infinity, -math::infinity), box.max);
        });
        set("Create a bounding box with volume", [] {
            Bounds box{Point{-1, -2, -3}, Point{3, 2, 1}};
            ASSERT_EQ_MSG("Inspect min", Point(-1, -2, -3), box.min);
            ASSERT_EQ_MSG("Inspect max", Point(3, 2, 1), box.max);
        });
    }

    void addition() {
        set("Addition", [] {
            set("Adding points to an empty box", [] {
                Bounds box;
                Point p1{-5, 2, 0};
                Point p2{7, 0, -3};
                box = box + p1 + p2;
                ASSERT_EQ_MSG("Inspect min", Point(-5, 0, -3), box.min);
                ASSERT_EQ_MSG("Inspect max", Point(7, 2, 0), box.max);
            });
        });
    }

    void of_shapes() {
        set("Bounding boxes of different shapes", [] {
            scenario("Sphere", [] {
                shapes::Sphere sphere;
                auto box = sphere.bounds();
                ASSERT_EQ(Bounds(Point(-1, -1, -1), Point(1, 1, 1)), box);
            });
            scenario("Plane", [] {
                shapes::Plane plane;
                auto box = plane.bounds();
                ASSERT_EQ(Bounds(Point(-math::infinity, 0, -math::infinity), Point(math::infinity, 0, math::infinity)), box);
            });
        });
    }
}