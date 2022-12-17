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
}