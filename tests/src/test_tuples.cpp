//
// Created by Melvic Ybanez on 12/5/22.
//

#include <cmath>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/math/include/tuples.h"

namespace rt::tests::tuples {
    static void identities();

    static void operations();

    static void magnitude();

    static void normalize();

    static void products();

    static void colors();

    static void reflections();

    void test() {
        set("Tuples", [] {
            identities();
            operations();
            magnitude();
            normalize();
            products();
            colors();
            reflections();
        });
    }

    void identities() {
        ASSERT_EQ_MSG("Default point", Point(), rt::Tuple(0, 0, 0, 1));
        ASSERT_EQ_MSG("Default vector", Vec(), rt::Tuple(0, 0, 0, 0));
        scenario("A tuple with W = 1.0 is a point", [] {
            Point point{4.3, -4.2, 3.1};
            ASSERT_EQ(point.w(), 1);
        });
        scenario("A tuple with W = 0.0 is a vector", [] {
            Vec vector{4.3, -4.2, 3.1};
            ASSERT_EQ(vector.w(), 0);
        });
    }

    void operations() {
        set("Addition", [] {
            scenario("Two tuples", [] {
                Point t1{3, -2, 5};
                Vec t2{-2, 3, 1};
                ASSERT_EQ(t1 + t2, Point(1, 1, 6));
            });
            scenario("Two vectors", [] {
                Vec v1{3, -2, 5};
                Vec v2{-2, 3, 1};
                ASSERT_EQ(v1 + v2, Vec(1, 1, 6));
            });
        });
        set("Subtraction", [] {
            scenario("Subtracting two points", [] {
                Point p1{3, 2, 1};
                Point p2{5, 6, 7};
                ASSERT_EQ(p1 - p2, Vec(-2, -4, -6));
            });
            scenario("Subtracting a vector from a point", [] {
                Point p{3, 2, 1};
                Vec v{5, 6, 7};
                ASSERT_EQ(p - v, Point(-2, -4, -6));
            });
            scenario("Subtracting two vectors", [] {
                Vec v1{3, 2, 1};
                Vec v2{5, 6, 7};
                ASSERT_EQ(v1 - v2, Vec(-2, -4, -6));
            });
        });
        scenario("Negating a tuple", [] {
            rt::Tuple t{1, -2, 3, -4};
            ASSERT_EQ(-t, rt::Tuple(-1, 2, -3, 4));
        });
        set("Multiplication/Division", [] {
            ASSERT_EQ_MSG("Multiplying a tuple by a scalar", rt::Tuple(1, -2, 3, -4) * 3.5,
                          rt::Tuple(3.5, -7, 10.5, -14));
            ASSERT_EQ_MSG("Multiplying a tuple by a fraction", rt::Tuple(1, -2, 3, -4) * 0.5,
                          rt::Tuple(0.5, -1, 1.5, -2));
            ASSERT_EQ_MSG("Dividing a tuple by a scalar", rt::Tuple(1, -2, 3, -4) / 2, rt::Tuple(0.5, -1, 1.5, -2));
        });
    }

    void magnitude() {
        set("Magnitude", [] {
            ASSERT_EQ_MSG("(1, 0, 0)", Vec(1, 0, 0).magnitude(), 1);
            ASSERT_EQ_MSG("(0, 1, 0)", Vec(0, 1, 0).magnitude(), 1);
            ASSERT_EQ_MSG("(0, 0, 1)", Vec(0, 0, 1).magnitude(), 1);
            ASSERT_EQ_MSG("(1, 2, 3)", Vec(1, 2, 3).magnitude(), std::sqrt(14));
            ASSERT_EQ_MSG("(-1, -2, -3)", Vec(-1, -2, -3).magnitude(), std::sqrt(14));
        });
    }

    void normalize() {
        set("Normalize", [] {
            ASSERT_EQ_MSG("(4, 0, 0)", Vec(4, 0, 0).normalize(), Vec(1, 0, 0));
            ASSERT_EQ_MSG("(1, 2, 3)", Vec(1, 2, 3).normalize(),
                          Vec(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
            ASSERT_EQ_MSG("The magnitude of normalized vector", Vec(1, 2, 3).normalize().magnitude(), 1);
        });
    }

    void products() {
        set("Products", [] {
            scenario("Dot", [] {
                Vec v1{1, 2, 3};
                Vec v2{2, 3, 4};
                ASSERT_EQ(v1.dot(v2), 20);
            });

            set("Cross", [] {
                Vec v1{1, 2, 3};
                Vec v2{2, 3, 4};
                ASSERT_EQ_MSG("First with Second", v1.cross(v2), Vec(-1, 2, -1));
                ASSERT_EQ_MSG("Second with First", v2.cross(v1), Vec(1, -2, 1));
            });
        });
    }

    void colors() {
        set("Colors", [] {
            set("Components", [] {
                rt::Color color{-0.5, 0.4, 1.7};
                ASSERT_EQ_MSG("Red", color.red(), -0.5);
                ASSERT_EQ_MSG("Green", color.green(), 0.4);
                ASSERT_EQ_MSG("Blue", color.blue(), 1.7);
            });
            set("Operations", [] {
                rt::Color c1{0.9, 0.6, 0.75};
                rt::Color c2{0.7, 0.1, 0.25};
                ASSERT_EQ_MSG("Addition", c1 + c2, rt::Color(1.6, 0.7, 1.0));
                ASSERT_EQ_MSG("Subtraction", c1 - c2, rt::Color(0.2, 0.5, 0.5));
                ASSERT_EQ_MSG("Multiplication", rt::Color(0.2, 0.3, 0.4) * 2, rt::Color(0.4, 0.6, 0.8));
                scenario("Hadamard product", [] {
                    rt::Color c1{1, 0.2, 0.4};
                    rt::Color c2{0.9, 1, 0.1};
                    ASSERT_EQ(c1 * c2, rt::Color(0.9, 0.2, 0.04));
                });
            });
        });
    }

    void reflections() {
        set("Reflections", [] {
            scenario("Reflecting a vector approaching at 45 degrees", [] {
                Vec v{1, -1, 0};
                Vec n{0, 1, 0};
                ASSERT_EQ(Vec(1, 1, 0), v.reflect(n));
            });
            scenario("Reflecting a vector off a slanted surface ", [] {
                Vec v{0, -1, 0};
                Vec n{std::sqrt(2) / 2, std::sqrt(2) / 2, 0};
                ASSERT_EQ(Vec(1, 0, 0), v.reflect(n));
            });
        });
    }
}