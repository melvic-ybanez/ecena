//
// Created by Melvic Ybanez on 12/16/22.
//

#include <array>
#include "../include/asserts.h"
#include "../include/test_transformations.h"
#include "../../math/include/matrix.h"

namespace tests::transformations {
    namespace matrix = rt::math::matrix;
    namespace math = rt::math;

    void all() {
        set("Transformations", [] {
            void translations();
        });
    }

    void translations() {
        set("Translations", [] {
            scenario("Multiplying by a translation matrix", [] {
                math::Matrix<4, 4> transform{matrix::translation(5, -3, 2)};
                auto point{rt::Point(-3, 4, 5)};
                ASSERT_EQ(transform * point, rt::Point(2, 1, 7));
            });
            scenario("Multiplying by the inverse of a translation matrix", [] {
                math::Matrix<4, 4> transform{matrix::translation(5, -3, 2)};
                math::Matrix<4, 4> inv{transform.inverse()};
                auto point{rt::Point{-3, 4, 5}};
                ASSERT_EQ(inv * point, rt::Point(-8, 7, 3));
            });
            scenario("Translation does not affect a vector", [] {
                math::Matrix<4, 4> transform{matrix::translation(5, -3, 2)};
                auto vector{rt::Vec{-3, 4, 5}};
                ASSERT_EQ(transform * vector, vector);
            });
        });
    }
}