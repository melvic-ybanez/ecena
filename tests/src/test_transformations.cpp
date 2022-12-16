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
            rotations();
        });
    }

    void translations() {
        set("Translations", [] {
            scenario("Multiplying by a translation matrix", [] {
                auto transform{matrix::translation(5, -3, 2)};
                rt::Point point{-3, 4, 5};
                ASSERT_EQ(transform * point, rt::Point(2, 1, 7));
            });
            scenario("Multiplying by the inverse of a translation matrix", [] {
                auto transform{matrix::translation(5, -3, 2)};
                auto inv{transform.inverse()};
                rt::Point point{-3, 4, 5};
                ASSERT_EQ(inv * point, rt::Point(-8, 7, 3));
            });
            scenario("Translation does not affect a vector", [] {
                math::Matrix<4, 4> transform{matrix::translation(5, -3, 2)};
                rt::Vec vector{-3, 4, 5};
                ASSERT_EQ(transform * vector, vector);
            });
        });
    }

    void scaling() {
        set("Scaling", [] {
            auto transform{matrix::scaling(2, 3, 4)};

            scenario("Applied to a point", [=] {
                rt::Point point{-4, 6, 8};
                ASSERT_EQ(transform * point, rt::Point(-8, 18, 32));
            });
            scenario("Applied to a vector", [=] {
                rt::Vec vector{-4, 6, 8};
                ASSERT_EQ(transform * vector, rt::Vec(-8, 18, 32));
            });
            scenario("Multiplying by the inverse of a scaling matrix", [=] {
                auto inv{transform.inverse()};
                rt::Vec vector{-4, 6, 8};
                ASSERT_EQ(inv * vector, rt::Vec(-2, 2, 2));
            });
            scenario("Reflection is scaling by a negative value", [] {
                auto transform{matrix::scaling(-1, 1, 1)};
                rt::Point point{2, 3, 4};
                ASSERT_EQ(transform * point, rt::Point(-2, 3, 4));
            });
        });
    }

    void rotations() {
        set("Rotations", [] {
            set("Rotating a point around the x-axis", [] {
                rt::Point point{0, 1, 0};
                auto half_quarter{matrix::rotation_x(math::pi / 4)};
                auto full_quarter{matrix::rotation_x(math::pi / 2)};

                ASSERT_EQ_MSG("Half quarter", half_quarter * point, rt::Point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", full_quarter * point, rt::Point(0, 0, 1));
            });
            scenario("The inverse of an x-rotation rotates in the opposite direction", [] {
                rt::Point point{0, 1, 0};
                auto half_quarter{matrix::rotation_x(math::pi / 4)};
                auto inv{half_quarter.inverse()};
                ASSERT_EQ(inv * point, rt::Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
            });
        });
    }
}