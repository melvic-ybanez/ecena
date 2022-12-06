//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/tuples.h"
#include "../include/asserts.h"
#include "../../include/tuple.h"

namespace tests::tuples {
    void all() {
        tuples::identities();
        tuples::operations();
    }

    void identities() {
        scenario("A tuple with W = 1.0 is a point", []() {
            auto point = rt::point(4.3, -4.2, 3.1);
            return point->w() == 1;
        });

        scenario("A tuple with W = 0.0 is a vector", []() {
            auto vector = rt::vec(4.3, -4.2, 3.1);
            return vector->w() == 0;
        });
    }

    void operations() {
        scenario("Adding two rt", []() {
            rt::Tuple<1> t1 = {3, -2, 5};
            rt::Tuple<0> t2 = {-2, 3, 1};
            return t1 + t2 == rt::Tuple<1>(1, 1, 6);
        });
    }
}