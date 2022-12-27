//
// Created by Melvic Ybanez on 12/16/22.
//

#include <array>
#include<cmath>
#include "../include/asserts.h"
#include "../include/test_transformations.h"
#include "../../engine/math/include/matrix.h"

namespace tests::transformations {
    namespace matrix = rt::math::matrix;
    namespace math = rt::math;

    void all() {
        set("Transformations", [] {
            translations();
            scaling();
            rotations();
            shearing();
            multiple_transformations();
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
                rt::Point point{2, 3, 4};
                ASSERT_EQ(point.scale(-1, 1, 1), rt::Point(-2, 3, 4));
            });
        });
    }

    void rotations() {
        set("Rotations", [] {
            auto half_quarter = math::pi / 4;
            auto full_quarter = math::pi / 2;

            set("Around the x-axis", [=] {
                rt::Point point{0, 1, 0};
                ASSERT_EQ_MSG("Half quarter", point.rotate_x(half_quarter),
                              rt::Point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", point.rotate_x(full_quarter), rt::Point(0, 0, 1));
            });
            scenario("The inverse of an x-rotation rotates in the opposite direction", [] {
                rt::Point point{0, 1, 0};
                auto half_quarter{matrix::rotation_x(math::pi / 4)};
                auto inv{half_quarter.inverse()};
                ASSERT_EQ(inv * point, rt::Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
            });
            set("Around the y-axis", [=] {
                rt::Point point{0, 0, 1};
                ASSERT_EQ_MSG("Half quarter", point.rotate_y(half_quarter),
                              rt::Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", point.rotate_y(full_quarter), rt::Point(1, 0, 0));
            });
            set("Around the z-axis", [=] {
                rt::Point point{0, 1, 0};
                ASSERT_EQ_MSG("Half quarter", point.rotate_z(half_quarter),
                              rt::Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
                ASSERT_EQ_MSG("Full quarter", point.rotate_z(full_quarter), rt::Point(-1, 0, 0));
            });
        });
    }

    void shearing() {
        set("Shearing", [] {
            rt::Point point{2, 3, 4};

            ASSERT_EQ_MSG("Moves x in proportion to y", point.shear(1, 0, 0, 0, 0, 0), rt::Point(5, 3, 4));
            ASSERT_EQ_MSG("Moves x in proportion to z", point.shear(0, 1, 0, 0, 0, 0), rt::Point(6, 3, 4));
            ASSERT_EQ_MSG("Moves y in the proportion to x", point.shear(0, 0, 1, 0, 0, 0), rt::Point(2, 5, 4));
            ASSERT_EQ_MSG("Moves y in the proportion to z", point.shear(0, 0, 0, 1, 0, 0), rt::Point(2, 7, 4));
            ASSERT_EQ_MSG("Moves z in the proportion to x", point.shear(0, 0, 0, 0, 1, 0), rt::Point(2, 3, 6));
            ASSERT_EQ_MSG("Moves z in the proportion to y", point.shear(0, 0, 0, 0, 0, 1), rt::Point(2, 3, 7));
        });
    }

    void multiple_transformations() {
        rt::Point point{1, 0, 1};
        auto rotation_x{math::matrix::rotation_x(math::pi / 2)};
        auto scaling{math::matrix::scaling(5, 5, 5)};
        auto translation{math::matrix::translation(10, 5, 7)};

        set("Individual transformations are applied in sequence", [=] {
            auto rotated{rotation_x * point};
            ASSERT_EQ_MSG("Apply rotation first", rotated, rt::Point(1, -1, 0));

            auto scaled{scaling * rotated};
            ASSERT_EQ_MSG("Then apply scaling", scaled, rt::Point(5, -5, 0));

            auto translated{translation * scaled};
            ASSERT_EQ_MSG("Then apply translation", translated, rt::Point(15, 0, 7));
        });
        scenario("Chained transformations must be applied in reverse order", [=] {
            auto transformation{translation * scaling * rotation_x};
            ASSERT_EQ(transformation * point, rt::Point(15, 0, 7));
        });
    }
}