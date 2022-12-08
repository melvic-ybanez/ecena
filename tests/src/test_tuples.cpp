//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/test_tuples.h"
#include "../include/asserts.h"
#include "../../include/tuples.h"

namespace tests::tuples {
    void all() {
        reset();
        set("Tuples", []() {
            identities();
            operations();
            magnitude();
            normalize();
        });
    }

    void identities() {
        ASSERT_EQ_MSG("Default point", rt::Point(), rt::Tuple(0, 0, 0, 1));
        ASSERT_EQ_MSG("Default vector", rt::Vec(), rt::Tuple(0, 0, 0, 0));
        scenario("A tuple with W = 1.0 is a point", [] {
            auto point = rt::Point(4.3, -4.2, 3.1);
            ASSERT_EQ(point.w(), 1);
        });
        scenario("A tuple with W = 0.0 is a vector", []() {
            auto vector = rt::Vec(4.3, -4.2, 3.1);
            ASSERT_EQ(vector.w(), 0);
        });
    }

    void operations() {
        set("Addition", []() {
            scenario("Two tuples", []() {
                rt::Point t1 = {3, -2, 5};
                rt::Vec t2 = {-2, 3, 1};
                ASSERT_EQ(t1 + t2, rt::Point(1, 1, 6));
            });
            scenario("Two vectors", []() {
                rt::Vec v1 = {3, -2, 5};
                rt::Vec v2 = {-2, 3, 1};
                ASSERT_EQ(v1 + v2, rt::Vec(1, 1, 6));
            });
        });
        set("Subtraction", []() {
            scenario("Subtracting two points", []() {
                rt::Point p1 = {3, 2, 1};
                rt::Point p2 = {5, 6, 7};
                ASSERT_EQ(p1 - p2, rt::Vec(-2, -4, -6));
            });
            scenario("Subtracting a vector from a point", []() {
                rt::Point p = {3, 2, 1};
                rt::Vec v = {5, 6, 7};
                ASSERT_EQ(p - v, rt::Point(-2, -4, -6));
            });
            scenario("Subtracting two vectors", []() {
                rt::Vec v1 = {3, 2, 1};
                rt::Vec v2 = {5, 6, 7};
                ASSERT_EQ(v1 - v2, rt::Vec(-2, -4, -6));
            });
        });
        scenario("Negating a tuple", []() {
            auto t = rt::Tuple(1, -2, 3, -4);
            ASSERT_EQ(-t, rt::Tuple(-1, 2, -3, 4));
        });
        set("Multiplication/Division", []() {
            ASSERT_EQ_MSG("Multiplying a tuple by a scalar", rt::Tuple(1, -2, 3, -4) * 3.5,
                          rt::Tuple(3.5, -7, 10.5, -14));
            ASSERT_EQ_MSG("Multiplying a tuple by a fraction", rt::Tuple(1, -2, 3, -4) * 0.5, rt::Tuple(0.5, -1, 1.5, -2));
            ASSERT_EQ_MSG("Dividing a tuple by a scalar", rt::Tuple(1, -2, 3, -4) / 2, rt::Tuple(0.5, -1, 1.5, -2));
        });
    }

    void magnitude() {
        set("Magnitude", []() {
            ASSERT_EQ_MSG("(1, 0, 0)", rt::Vec(1, 0, 0).magnitude(), 1);
            ASSERT_EQ_MSG("(0, 1, 0)", rt::Vec(0, 1, 0).magnitude(), 1);
            ASSERT_EQ_MSG("(0, 0, 1)", rt::Vec(0, 0, 1).magnitude(), 1);
            ASSERT_EQ_MSG("(1, 2, 3)", rt::Vec(1, 2, 3).magnitude(), std::sqrt(14));
            ASSERT_EQ_MSG("(-1, -2, -3)", rt::Vec(-1, -2, -3).magnitude(), std::sqrt(14));
        });
    }

    void normalize() {
        set("Normalize", []() {
            ASSERT_EQ_MSG("(4, 0, 0)", rt::Vec(4, 0, 0).normalize(), rt::Vec(1, 0, 0));
            ASSERT_EQ_MSG("(1, 2, 3)", rt::Vec(1, 2, 3).normalize(), rt::Vec(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
            ASSERT_EQ_MSG("The magnitude of normalized vector", rt::Vec(1, 2, 3).normalize().magnitude(), 1);
        });
    }
}