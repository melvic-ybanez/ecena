//
// Created by Melvic Ybanez on 12/16/22.
//

#include <array>
#include<cmath>
#include "../include/asserts.h"
#include "../include/tests.h"
#include "../../engine/math/include/matrix.h"

namespace rt::tests::transformations {
    static void translations();

    static void scaling();

    static void rotations();

    static void shearing();

    static void multiple_transformations();

    static void view_transformations();

    void test() {
        set("Transformations", [] {
            translations();
            scaling();
            rotations();
            shearing();
            multiple_transformations();
            view_transformations();
        });
    }

    void translations() {
        set("Translations", [] {
            scenario("Multiplying by a translation matrix", [] {
                Point point{-3, 4, 5};
                ASSERT_EQ(point.translate(5, -3, 2), Point(2, 1, 7));
            });

            auto transform{matrix::translation(5, -3, 2)};
            scenario("Multiplying by the inverse of a translation matrix", [=] {
                auto inv{transform.inverse()};
                Point point{-3, 4, 5};
                ASSERT_EQ(inv * point, Point(-8, 7, 3));
            });
            scenario("Translation does not affect a vector", [=] {
                Vec vector{-3, 4, 5};
                ASSERT_EQ(transform * vector, vector);
            });
        });
    }

    void scaling() {
        set("Scaling", [] {
            auto transform{matrix::scaling(2, 3, 4)};

            scenario("Applied to a point", [=] {
                Point point{-4, 6, 8};
                ASSERT_EQ(transform * point, Point(-8, 18, 32));
            });
            scenario("Applied to a vector", [=] {
                Vec vector{-4, 6, 8};
                ASSERT_EQ(transform * vector, Vec(-8, 18, 32));
            });
            scenario("Multiplying by the inverse of a scaling matrix", [=] {
                auto inv{transform.inverse()};
                Vec vector{-4, 6, 8};
                ASSERT_EQ(inv * vector, Vec(-2, 2, 2));
            });
            scenario("Reflection is scaling by a negative value", [] {
                Point point{2, 3, 4};
                ASSERT_EQ(point.scale(-1, 1, 1), Point(-2, 3, 4));
            });
        });
    }

    void rotations() {
        set("Rotations", [] {
            auto half_quarter = math::pi / 4;
            auto full_quarter = math::pi / 2;

            set("Around the x-axis", [=] {
                Point point{0, 1, 0};
                ASSERT_EQ_MSG("Half quarter", point.rotate_x(half_quarter),
                              Point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", point.rotate_x(full_quarter), Point(0, 0, 1));
            });
            scenario("The inverse of an x-rotation rotates in the opposite direction", [] {
                Point point{0, 1, 0};
                auto half_quarter{matrix::rotation_x(math::pi / 4)};
                auto inv{half_quarter.inverse()};
                ASSERT_EQ(inv * point, Point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
            });
            set("Around the y-axis", [=] {
                Point point{0, 0, 1};
                ASSERT_EQ_MSG("Half quarter", point.rotate_y(half_quarter),
                              Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
                ASSERT_EQ_MSG("Full quarter", point.rotate_y(full_quarter), Point(1, 0, 0));
            });
            set("Around the z-axis", [=] {
                Point point{0, 1, 0};
                ASSERT_EQ_MSG("Half quarter", point.rotate_z(half_quarter),
                              Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
                ASSERT_EQ_MSG("Full quarter", point.rotate_z(full_quarter), Point(-1, 0, 0));
            });
        });
    }

    void shearing() {
        set("Shearing", [] {
            Point point{2, 3, 4};

            ASSERT_EQ_MSG("Moves x in proportion to y", point.shear(1, 0, 0, 0, 0, 0), Point(5, 3, 4));
            ASSERT_EQ_MSG("Moves x in proportion to z", point.shear(0, 1, 0, 0, 0, 0), Point(6, 3, 4));
            ASSERT_EQ_MSG("Moves y in the proportion to x", point.shear(0, 0, 1, 0, 0, 0), Point(2, 5, 4));
            ASSERT_EQ_MSG("Moves y in the proportion to z", point.shear(0, 0, 0, 1, 0, 0), Point(2, 7, 4));
            ASSERT_EQ_MSG("Moves z in the proportion to x", point.shear(0, 0, 0, 0, 1, 0), Point(2, 3, 6));
            ASSERT_EQ_MSG("Moves z in the proportion to y", point.shear(0, 0, 0, 0, 0, 1), Point(2, 3, 7));
        });
    }

    void multiple_transformations() {
        Point point{1, 0, 1};
        auto rotation_x{matrix::rotation_x(math::pi / 2)};
        auto scaling{matrix::scaling(5, 5, 5)};
        auto translation{matrix::translation(10, 5, 7)};

        set("Individual transformations are applied in sequence", [=] {
            auto rotated{rotation_x * point};
            ASSERT_EQ_MSG("Apply rotation first", rotated, Point(1, -1, 0));

            auto scaled{scaling * rotated};
            ASSERT_EQ_MSG("Then apply scaling", scaled, Point(5, -5, 0));

            auto translated{translation * scaled};
            ASSERT_EQ_MSG("Then apply translation", translated, Point(15, 0, 7));
        });
        scenario("Chained transformations must be applied in reverse order", [=] {
            auto transformation{translation * scaling * rotation_x};
            ASSERT_EQ(transformation * point, Point(15, 0, 7));
        });
    }

    void view_transformations() {
        set("View transformations", [] {
            scenario("For the default orientation", [] {
                Point from{0, 0, 0};
                Point to{0, 0, -1};
                Vec up{0, 1, 0};
                auto idm = matrix::identity<4, 4>();
                auto vt = matrix::view_transform(from, to, up);
                ASSERT_EQ(idm, vt);
            });
            scenario("Looking in positive z direction", [] {
                Point from{0, 0, 0};
                Point to{0, 0, 1};
                Vec up{0, 1, 0};
                auto vt = matrix::view_transform(from, to, up);
                ASSERT_EQ(matrix::scaling(-1, 1, -1), vt);
            });
            scenario("The view transformation moves the world", [] {
                Point from{0, 0, 8};
                Point to{0, 0, 0};
                Vec up{0, 1, 0};
                auto vt = matrix::view_transform(from, to, up);
                ASSERT_EQ(matrix::translation(0, 0, -8), vt);
            });
            scenario("An arbitrary view transformation", [] {
                Point from{1, 3, 2};
                Point to{4, -2, 8};
                Vec up{1, 1, 0};
                auto vt = matrix::view_transform(from, to, up);
                Matrix<4, 4> result{
                        {{{-0.50709, 0.50709, 0.67612, -2.36643},
                          {0.76772, 0.60609, 0.12122, -2.82843},
                          {-0.35857, 0.59761, -0.71714, 0.00000},
                          {0.00000, 0.00000, 0.00000, 1.00000}}}
                };
            });
        });
    }
}