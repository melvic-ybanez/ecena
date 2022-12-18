//
// Created by Melvic Ybanez on 12/19/22.
//

#include <array>
#include "../include/test_intersections.h"
#include "../include/asserts.h"
#include "../../core/include/intersection.h"

namespace tests::intersections {
    void all() {
        set("Intersections", [] {
            init();
        });
    }

    void init() {
        scenario("An intersection encapsulates t and object", [] {
            auto *sphere = new rt::shapes::Sphere;
            rt::Intersection i{3.5, sphere};
            ASSERT_EQ(3.5, i.t());
        });
    }
}