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
            shearing();
        });
    }

    void translations() {
        set("Translations", [] {
            scenario("Multiplying by a translation matrix", [] {
                rt::Point point{-3, 4, 5};
                ASSERT_EQ(point.translate(5, -3, 2), rt::Point(2, 1, 7));
            });

            auto transform{matrix::translation(5, -3, 2)};
            scenario("Multiplying by the inverse of a translation matrix", [=] {
                auto inv{transform.inverse()};
                rt::Point point{-3, 4, 5};
                ASSERT_EQ(inv * point, rt::Point(-8, 7, 3));
            });
            scenario("Translation does not affect a vector", [=] {
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
            set("Around the x-axis", [] {
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
            set("Around the y-axis", [] {
                rt::Point point{0, 0, 1};
                auto half_quarter{matrix::rotation_y(math::pi / 4)};
                auto full_quarter{matrix::rotation_y(math::pi / 2)};

                ASSERT_EQ_MSG("Half quarter", half_quarter * point, rt::Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", full_quarter * point, rt::Point(1, 0, 0));
            });
            set("Around the z-axis", [] {
                rt::Point point{0, 1, 0};
                auto half_quarter{matrix::rotation_z(math::pi / 4)};
                auto full_quarter{matrix::rotation_z(math::pi / 2)};

                ASSERT_EQ_MSG("Half quarter", half_quarter * point, rt::Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
                ASSERT_EQ_MSG("Full quarter", full_quarter * point, rt::Point(-1, 0, 0));
            });
        });
    }

    void shearing() {
        set("Shearing", [] {
            rt::Point point{2, 3, 4};

            scenario("Moves x in proportion to y", [=] {
                auto transform{matrix::shearing(1, 0, 0, 0, 0, 0)};
                ASSERT_EQ(transform * point, rt::Point(5, 3, 4));
            });
            scenario("Moves x in proportion to z", [=] {
                auto transform{matrix::shearing(0, 1, 0, 0, 0, 0)};
                ASSERT_EQ(transform * point, rt::Point(6, 3, 4));
            });
            scenario("Moves y in the proportion to x", [=] {
                auto transform{matrix::shearing(0, 0, 1, 0, 0, 0)};
                ASSERT_EQ(transform * point, rt::Point(2, 5, 4));
            });
            scenario("Moves y in the proportion to z", [=] {
                auto transform{matrix::shearing(0, 0, 0, 1, 0, 0)};
                ASSERT_EQ(transform * point, rt::Point(2, 7, 4));
            });
            scenario("Moves z in the proportion to x", [=] {
                auto transform{matrix::shearing(0, 0, 0, 0, 1, 0)};
                ASSERT_EQ(transform * point, rt::Point(2, 3, 6));
            });
            scenario("Moves z in the proportion to y", [=] {
                auto transform{matrix::shearing(0, 0, 0, 0, 0, 1)};
                ASSERT_EQ(transform * point, rt::Point(2, 3, 7));
            });
        });
    }
}