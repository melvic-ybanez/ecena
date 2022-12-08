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
            tuples::identities();
            tuples::operations();
        });
    }

    void identities() {
        assert_equals("Default point", rt::Point(), rt::Tuple(0, 0, 0, 1));
        assert_equals("Default vector", rt::Vec(), rt::Tuple(0, 0, 0, 0));
        scenario("A tuple with W = 1.0 is a point", [] {
            auto point = rt::Point(4.3, -4.2, 3.1);
            assert_equals(point.w(), 1);
        });
        scenario("A tuple with W = 0.0 is a vector", []() {
            auto vector = rt::Vec(4.3, -4.2, 3.1);
            assert_equals(vector.w(), 0);
        });
    }

    void operations() {
        set("Addition", []() {
            scenario("Two tuples", []() {
                rt::Point t1 = {3, -2, 5};
                rt::Vec t2 = {-2, 3, 1};
                assert_equals(t1 + t2, rt::Point(1, 1, 6));
            });
            scenario("Two vectors", []() {
                rt::Vec v1 = {3, -2, 5};
                rt::Vec v2 = {-2, 3, 1};
                assert_equals(v1 + v2, rt::Vec(1, 1, 6));
            });
        });
        set("Subtraction", []() {
            scenario("Subtracting two points", []() {
                rt::Point p1 = {3, 2, 1};
                rt::Point p2 = {5, 6, 7};
                assert_equals(p1 - p2, rt::Vec(-2, -4, -6));
            });
            scenario("Subtracting a vector from a point", []() {
                rt::Point p = {3, 2, 1};
                rt::Vec v = {5, 6, 7};
                assert_equals(p - v, rt::Point(-2, -4, -6));
            });
            scenario("Subtracting two vectors", []() {
                rt::Vec v1 = {3, 2, 1};
                rt::Vec v2 = {5, 6, 7};
                assert_equals(v1 - v2, rt::Vec(-2, -4, -6));
            });
        });
        scenario("Negating a tuple", []() {
            auto t = rt::Tuple(1, -2, 3, -4);
            assert_equals(-t, rt::Tuple(-1, 2, -3, 4));
        });
        set("Multiplication", []() {
            assert_equals("Multiplying a tuple by a scalar", rt::Tuple(1, -2, 3, -4) * 3.5,
                          rt::Tuple(3.5, -7, 10.5, -14));
        });
    }
}