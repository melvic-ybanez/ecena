//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/tuples.h"
#include "../include/asserts.h"
#include "../../include/tuple.h"

namespace tests::tuples {
    void all() {
        reset_counts();
        tuples::identities();
        tuples::operations();
    }

    void identities() {
        scenario("A tuple with W = 1.0 is a point", []() {
            auto point = rt::point(4.3, -4.2, 3.1);
            return point.w() == 1;
        });

        scenario("A tuple with W = 0.0 is a vector", []() {
            auto vector = rt::vec(4.3, -4.2, 3.1);
            return vector.w() == 0;
        });
    }

    void operations() {
        scenario("Adding two tuples", []() {
            rt::Tuple<1> t1 = {3, -2, 5};
            rt::Tuple<0> t2 = {-2, 3, 1};
            return t1 + t2 == rt::Tuple<1>(1, 1, 6);
        });
        scenario("Adding two vectors", []() {
            rt::Vec v1 = {3, -2, 5};
            rt::Vec v2 = {-2, 3, 1};
            return v1 + v2 == rt::vec(1, 1, 6);
        });
        scenario("Subtracting two points", []() {
            rt::Point p1 = {3, 2, 1};
            rt::Point p2 = {5, 6, 7};
            return p1 - p2 == rt::vec(-2, -4, -6);
        });
        scenario("Subtracting a vector from a point", []() {
            rt::Point p = {3, 2, 1};
            rt::Vec v = {5, 6, 7};
            return p - v == rt::point(-2, -4, -6);
        });
        scenario("Subtracting two vectors", []() {
            rt::Vec v1 = {3, 2, 1};
            rt::Vec v2 = {5, 6, 7};
            return v1 - v2 == rt::vec(-2, -4, -6);
        });
        scenario("Negating a tuple", []() {
            auto t = rt::Tuple<-4>(1, -2, 3);
            return -t == rt::Tuple<4>(-1, 2, -3);
        });
    }
}