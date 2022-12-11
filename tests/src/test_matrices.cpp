//
// Created by Melvic Ybanez on 12/11/22.
//

#include <array>
#include "../include/test_matrices.h"
#include "../include/asserts.h"
#include "../../math/include/matrix.h"

namespace tests::matrices {
    using namespace rt;

    void all() {
        set("Matrices", [] {
            init();
            compare();
            multiplication();
        });
    }

    void init() {
        set("4 x 4 Matrix", [] {
            math::MatrixTable<4, 4> table{
                    {{1, 2, 3, 4},
                     {5.5, 6.5, 7.5, 8.5},
                     {9, 10, 11, 12},
                     {13.5, 14.5, 15.5, 16.5}}};
            math::Matrix<4, 4> matrix{table};

            ASSERT_EQ_MSG("[0, 0]", 1, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 3]", 4, matrix[0][3]);
            ASSERT_EQ_MSG("[1, 0]", 5.5, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 2]", 7.5, matrix[1][2]);
            ASSERT_EQ_MSG("[2, 2]", 11, matrix[2][2]);
            ASSERT_EQ_MSG("[3, 0]", 13.5, matrix[3][0]);
            ASSERT_EQ_MSG("[3, 2]", 15.5, matrix[3][2]);
        });
        set("2 x 2 Matrix", [] {
            math::MatrixTable<2, 2> table{
                    {{-3, 5},
                     {1, -2}}};
            math::Matrix<2, 2> matrix{table};

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 1]", 5, matrix[0][1]);
            ASSERT_EQ_MSG("[1, 0]", 1, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
        });
        set("3 x 3 Matrix", [] {
            math::MatrixTable<3, 3> table{
                    {{-3, 5, 0},
                     {1, -2, -7},
                     {0, 1, 1}}};
            math::Matrix<3, 3> matrix{table};

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
            ASSERT_EQ_MSG("[2, 2]", 1, matrix[2][2]);
        });
    }

    void compare() {
        set("Comparing matrices", [] {
            scenario("Identical matrices", [] {
                math::MatrixTable<4, 4> t1{
                        {{1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 8, 7, 6},
                         {5, 4, 3, 2}}};
                math::MatrixTable<4, 4> t2{
                        {{1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 8, 7, 6},
                         {5, 4, 3, 2}}};

                math::Matrix<4, 4> m1{t1};
                math::Matrix<4, 4> m2{t2};

                ASSERT_EQ(m1, m2);
            });
            scenario("Non-identical matrices", [] {
                math::MatrixTable<4, 4> t1{
                        {{1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 8, 7, 6},
                         {5, 4, 3, 2}}};
                math::MatrixTable<4, 4> t2{
                        {{2, 3, 4, 5},
                         {6, 7, 8, 9},
                         {8, 7, 6, 5},
                         {4, 3, 2, 1}}};

                math::Matrix<4, 4> m1{t1};
                math::Matrix<4, 4> m2{t2};

                ASSERT_TRUE(m1 != m2);
            });
        });
    }

    void multiplication() {
        set("Multiplication", [] {
            scenario("Two matrices", [] {
                math::MatrixTable<4, 4> t1{
                        {{1, 2, 3, 4},
                         {5, 6, 7, 8},
                         {9, 8, 7, 6},
                         {5, 4, 3, 2}}
                };
                math::MatrixTable<4, 4> t2{
                        {{-2, 1, 2, 3},
                         {3, 2, 1, -1},
                         {4, 3, 6, 5},
                         {1, 2, 7, 8}}
                };

                math::MatrixTable<4, 4> et{
                        {{20, 22, 50, 48},
                         {44, 54, 114, 108},
                         {40, 58, 110, 102},
                         {16, 26, 46, 42}}
                };
                math::Matrix<4, 4> expected{et};
                math::Matrix<4, 4> product = math::Matrix<4, 4>{t1} * math::Matrix<4, 4>{t2};

                ASSERT_EQ(expected, product);
            });
        });
    }
}