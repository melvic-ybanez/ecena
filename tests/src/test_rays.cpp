//
// Created by Melvic Ybanez on 12/17/22.
//

#include "../include/test_rays.h"
#include "../include/asserts.h"
#include "../../core/include/tuples.h"
#include "../../core/include/ray.h"

namespace tests::rays {
    void all() {
        set("Rays", [] {
            init();
            distance();
        });
    }

    void init() {
        set("Creating a ray", [] {
            rt::Point origin{1, 2, 3};
            rt::Vec direction{4, 5, 6};
            rt::Ray ray{origin, direction};
            ASSERT_EQ_MSG("Origin", origin, ray.origin());
            ASSERT_EQ_MSG("Direction", direction, ray.direction());
        });
    }

    void distance() {
        set("Computing a point from a distance", [] {
            rt::Ray ray{rt::Point{2, 3, 4}, rt::Vec{1, 0, 0}};
            ASSERT_EQ_MSG("at(0)", ray.at(0), rt::Point(2, 3, 4));
            ASSERT_EQ_MSG("at(1)", ray.at(1), rt::Point(3, 3, 4));
            ASSERT_EQ_MSG("at(-1)", ray.at(-1), rt::Point(1, 3, 4));
            ASSERT_EQ_MSG("at(2.5)", ray.at(2.5), rt::Point(4.5, 3, 4));
        });
    }
}