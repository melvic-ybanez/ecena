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
            translations();
            scaling();
        });
    }

    void translations() {
        set("Translations", [] {
            scenario("Multiplying by a translation matrix", [] {
                auto transform{matrix::translation(5, -3, 2)};
                auto point{rt::Point{-3, 4, 5}};
                ASSERT_EQ(transform * point, rt::Point(2, 1, 7));
            });
            scenario("Multiplying by the inverse of a translation matrix", [] {
                auto transform{matrix::translation(5, -3, 2)};
                auto inv{transform.inverse()};
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

    void scaling() {
        set("Scaling", [] {
            auto transform{matrix::scaling(2, 3, 4)};

            scenario("Applied to a point", [=] {
                auto point{rt::Point{-4, 6, 8}};
                ASSERT_EQ(transform * point, rt::Point(-8, 18, 32));
            });
            scenario("Applied to a vector", [=] {
                auto vector{rt::Vec{-4, 6, 8}};
                ASSERT_EQ(transform * vector, rt::Vec(-8, 18, 32));
            });
            scenario("Multiplying by the inverse of a scaling matrix", [=] {
                auto inv{transform.inverse()};
                auto vector{rt::Vec{-4, 6, 8}};
                ASSERT_EQ(inv * vector, rt::Vec(-2, 2, 2));
            });
            scenario("Reflection is scaling by a negative value", [] {
                auto transform{matrix::scaling(-1, 1, 1)};
                auto point{rt::Point{2, 3, 4}};
                ASSERT_EQ(transform * point, rt::Point(-2, 3, 4));
            });
        });
    }
}