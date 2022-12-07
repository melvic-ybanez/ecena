//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/tuples.h"
#include "../include/asserts.h"
#include "../../include/tuple.h"

namespace tests::tuples {
    void all() {
        reset_counts();
        set("Tuples", []() {
            tuples::identities();
            tuples::operations();
        });
    }

    void identities() {
        assert_equals("Default point", rt::Point(), rt::Tuple(0, 0, 0, 1));
        assert_equals("Default vector", rt::Vec(), rt::Tuple(0, 0, 0, 0));
        assert_with("A tuple with W = 1.0 is a point", [](const std::string &msg) {
            auto point = rt::Point(4.3, -4.2, 3.1);
            assert_equals(msg, point.w(), 1);
        });
        assert_with("A tuple with W = 0.0 is a vector", [](const std::string &msg) {
            auto vector = rt::Vec(4.3, -4.2, 3.1);
            assert_equals(msg, vector.w(), 0);
        });
    }

    void operations() {
        set("Addition", []() {
            assert_with("Two tuples", [](const std::string &msg) {
                rt::Point t1 = {3, -2, 5};
                rt::Vec t2 = {-2, 3, 1};
                assert_equals(msg, t1 + t2, rt::Point(1, 1, 6));
            });
            assert_with("Two vectors", [](const std::string &msg) {
                rt::Vec v1 = {3, -2, 5};
                rt::Vec v2 = {-2, 3, 1};
                assert_equals(msg, v1 + v2, rt::Vec(1, 1, 6));
            });
        });
        set("Subtraction", []() {
            assert_with("Subtracting two points", [](const std::string &msg) {
                rt::Point p1 = {3, 2, 1};
                rt::Point p2 = {5, 6, 7};
                assert_equals(msg, p1 - p2, rt::Vec(-2, -4, -6));
            });
            assert_with("Subtracting a vector from a point", [](const std::string &msg) {
                rt::Point p = {3, 2, 1};
                rt::Vec v = {5, 6, 7};
                assert_equals(msg, p - v, rt::Point(-2, -4, -6));
            });
            assert_with("Subtracting two vectors", [](const std::string &msg) {
                rt::Vec v1 = {3, 2, 1};
                rt::Vec v2 = {5, 6, 7};
                assert_equals(msg, v1 - v2, rt::Vec(-2, -4, -6));
            });
        });
        assert_with("Negating a tuple", [](const std::string &msg) {
            auto t = rt::Tuple(1, -2, 3, -4);
            assert_equals(msg, -t, rt::Tuple(-1, 2, -3, 4));
        });
        assert_equals("Multiplying a tuple by a scalar", rt::Tuple(1, -2, 3, -4) * 3.5, rt::Tuple(3.5, -7, 10.5, -14));
    }
}