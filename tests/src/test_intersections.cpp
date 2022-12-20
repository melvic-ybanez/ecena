//
// Created by Melvic Ybanez on 12/19/22.
//

#include <array>
#include "../include/test_intersections.h"
#include "../include/asserts.h"
#include "../../core/include/intersection.h"
#include "../../core/include/shapes.h"

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
            ASSERT_EQ_MSG("Inspect t", 3.5, i.t());
            ASSERT_EQ_MSG("Inspect object", rt::shapes::Type::sphere, sphere->type());
        });
    }
}