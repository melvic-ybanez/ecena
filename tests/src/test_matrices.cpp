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
            transpose();
        });
    }

    void init() {
        set("4 x 4 Matrix", [] {
            math::Matrix<4, 4> matrix{
                    {{{1, 2, 3, 4},
                      {5.5, 6.5, 7.5, 8.5},
                      {9, 10, 11, 12},
                      {13.5, 14.5, 15.5, 16.5}}}
            };

            ASSERT_EQ_MSG("[0, 0]", 1, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 3]", 4, matrix[0][3]);
            ASSERT_EQ_MSG("[1, 0]", 5.5, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 2]", 7.5, matrix[1][2]);
            ASSERT_EQ_MSG("[2, 2]", 11, matrix[2][2]);
            ASSERT_EQ_MSG("[3, 0]", 13.5, matrix[3][0]);
            ASSERT_EQ_MSG("[3, 2]", 15.5, matrix[3][2]);
        });
        set("2 x 2 Matrix", [] {
            math::Matrix<2, 2> matrix{
                    {{{-3, 5},
                      {1, -2}}}
            };

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 1]", 5, matrix[0][1]);
            ASSERT_EQ_MSG("[1, 0]", 1, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
        });
        set("3 x 3 Matrix", [] {
            math::Matrix<3, 3> matrix{
                    {{{-3, 5, 0},
                      {1, -2, -7},
                      {0, 1, 1}}}
            };

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
            ASSERT_EQ_MSG("[2, 2]", 1, matrix[2][2]);
        });
    }

    void compare() {
        set("Comparing matrices", [] {
            scenario("Identical matrices", [] {
                math::Matrix<4, 4> m1{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };
                math::Matrix<4, 4> m2{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };

                ASSERT_EQ(m1, m2);
            });
            scenario("Non-identical matrices", [] {
                math::Matrix<4, 4> m1{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };
                math::Matrix<4, 4> m2{
                        {{{2, 3, 4, 5},
                          {6, 7, 8, 9},
                          {8, 7, 6, 5},
                          {4, 3, 2, 1}}}
                };

                ASSERT_TRUE(m1 != m2);
            });
        });
    }

    void multiplication() {
        set("Multiplication", [] {
            scenario("Matrix-matrix", [] {
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
                math::Matrix<4, 4> expected{
                        {{{20, 22, 50, 48},
                          {44, 54, 114, 108},
                          {40, 58, 110, 102},
                          {16, 26, 46, 42}}}
                };
                auto product = math::Matrix<4, 4>{t1} * math::Matrix<4, 4>{t2};

                ASSERT_EQ(expected, product);
            });
            scenario("Matrix-tuple", [] {
                math::Matrix<4, 4> matrix{
                        {{{1, 2, 3, 4},
                          {2, 4, 4, 2},
                          {8, 6, 4, 1},
                          {0, 0, 0, 1}}}
                };
                rt::Tuple tuple{1, 2, 3, 1};

                rt::Tuple expected{18, 24, 33, 1};

                ASSERT_EQ(expected, matrix * tuple);
            });
            scenario("Matrix-Id", [] {
                math::Matrix<4, 4> matrix{
                        {{{0, 1, 2, 4},
                          {1, 2, 4, 8},
                          {2, 4, 8, 6},
                          {4, 8, 16, 32}}}
                };
                auto result = matrix * math::identity<4, 4>();
                ASSERT_EQ(matrix, result);
            });
        });
    }

    void transpose() {
        set("Transpose", [] {
            scenario("A non-id matrix", [] {
                math::Matrix<4, 4> matrix{
                        {{{0, 9, 3, 0},
                          {9, 8, 0, 8},
                          {1, 8, 5, 3},
                          {0, 0, 5, 8}}}
                };
                math::Matrix<4, 4> expected{
                        {{{0, 9, 1, 0},
                          {9, 8, 8, 0},
                          {3, 0, 5, 5},
                          {0, 8, 3, 8}}}
                };
                ASSERT_EQ(expected, matrix.transpose());
            });
            scenario("An identity matrix", [] {
               auto idm = math::identity<4, 4>();
                ASSERT_EQ(idm, idm.transpose());
            });
        });
    }
}