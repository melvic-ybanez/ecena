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
            aggregations();
        });
    }

    void init() {
        scenario("An intersection encapsulates t and object", [] {
            auto sphere = new rt::shapes::Sphere;
            rt::Intersection i{3.5, sphere};
            ASSERT_EQ_MSG("Inspect t", 3.5, i.t());
            ASSERT_EQ_MSG("Inspect object", rt::shapes::Type::sphere, sphere->type());
        });
    }

    void aggregations() {
        set("Aggregating intersections", [] {
            auto sphere = new rt::shapes::Sphere;
            auto i1 = new rt::Intersection{1, sphere};
            auto i2 = new rt::Intersection{2, sphere};
            rt::intersections::Aggregate agg{{i1, i2}};

            ASSERT_EQ_MSG("Inspect count", 2, agg.count());
            ASSERT_EQ_MSG("Inspect first element", i1, agg[0]);
            ASSERT_EQ_MSG("Inspect second element", i2, agg[1]);
        });
    }
}