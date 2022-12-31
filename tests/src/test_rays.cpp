//
// Created by Melvic Ybanez on 12/17/22.
//

#include "../include/test_rays.h"
#include "../include/asserts.h"
#include "../../engine/include/ray.h"

namespace rt::tests::rays {
    void all() {
        set("Rays", [] {
            init();
            distance();
            transformations();
        });
    }

    void init() {
        set("Creating a ray", [] {
            Point origin{1, 2, 3};
            Vec direction{4, 5, 6};
            Ray ray{origin, direction};
            ASSERT_EQ_MSG("Origin", origin, ray.origin);
            ASSERT_EQ_MSG("Direction", direction, ray.direction);
        });
    }

    void distance() {
        set("Computing a point from a distance", [] {
            Ray ray{Point{2, 3, 4}, Vec{1, 0, 0}};
            ASSERT_EQ_MSG("at(0)", ray.at(0), Point(2, 3, 4));
            ASSERT_EQ_MSG("at(1)", ray.at(1), Point(3, 3, 4));
            ASSERT_EQ_MSG("at(-1)", ray.at(-1), Point(1, 3, 4));
            ASSERT_EQ_MSG("at(2.5)", ray.at(2.5), Point(4.5, 3, 4));
        });
    }

    void transformations() {
        set("Transformations", [] {
            Ray ray{Point{1, 2, 3}, Vec{0, 1, 0}};
            scenario("Translation", [=] {
                auto ray2 = ray.translate(3, 4, 5);
                ASSERT_EQ(Ray(Point(4, 6, 8), Vec(0, 1, 0)), ray2);
            });
            scenario("Scaling", [=] {
                auto ray2 = ray.scale(2, 3, 4);
                ASSERT_EQ(Ray(Point(2, 6, 12), Vec(0, 3, 0)), ray2);
            });
            scenario("Scaling", [=] {
                auto ray2 = ray.scale(2, 3, 4);
                ASSERT_EQ(Ray(Point(2, 6, 12), Vec(0, 3, 0)), ray2);
            });
        });
    }
}