//
// Created by Melvic Ybanez on 1/4/23.
//

#include "../include/asserts.h"
#include "../../engine/include/shapes.h"
#include "../include/tests.h"

namespace rt::tests::planes {
    static void normal();

    static void intersections();

    void test() {
        set("Planes", [] {
            normal();
            intersections();
        });
    }

    void normal() {
        set("Normal", [] {
            scenario("Should be constant everywhere", [] {
                shapes::Plane plane;
                auto n1 = plane.local_normal_at({0, 0, 0});
                auto n2 = plane.local_normal_at({10, 0, -10});
                auto n3 = plane.local_normal_at({-5, 0, 150});

                Vec expected{0, 1, 0};

                ASSERT_TRUE(n1 == expected && n2 == expected && n3 == expected);
            });
        });
    }

    void intersections() {
        set("Intersections", [] {
            scenario("With a ray parallel to the plane", [] {
                shapes::Plane plane;
                Ray ray{{0, 10, 0},
                        {0, 0,  1}};
                auto xs = plane.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            scenario("With a coplanar ray", [] {
                shapes::Plane plane;
                Ray ray{{0, 0, 0},
                        {0, 0, 1}};
                auto xs = plane.local_intersect(ray);

                // technically, there should be infinite number of intersections but the
                // ray is too thin that it's practically invisible in this case
                ASSERT_TRUE(xs.empty());
            });
            set("With a ray intersecting a plan from above", [] {
                shapes::Plane plane;
                Ray ray{{0, 1,  0},
                        {0, -1, 0}};
                auto xs = plane.local_intersect(ray);

                ASSERT_EQ(1, xs.count());
                ASSERT_EQ(1, xs[0]->t);
                ASSERT_EQ(&plane, xs[0]->object);
            });
            set("With a ray intersecting a plan from below", [] {
                shapes::Plane plane;
                Ray ray{{0, -1, 0},
                        {0, 1,  0}};
                auto xs = plane.local_intersect(ray);

                ASSERT_EQ(1, xs.count());
                ASSERT_EQ(1, xs[0]->t);
                ASSERT_EQ(&plane, xs[0]->object);
            });
        });
    }
}
