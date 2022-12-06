//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/tuples.h"
#include "../include/asserts.h"
#include "../../include/tuple.h"

namespace tests {
    void tuples() {
        assert_function("Adding two tuples", []() {
            tuples::Tuple<1> t1 = {3, -2, 5};
            tuples::Tuple<0> t2 = {-2, 3, 1};
            return t1 + t2 == tuples::Tuple<1>(1, 1, 6);
        });
    }
}