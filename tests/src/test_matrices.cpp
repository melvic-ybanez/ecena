//
// Created by Melvic Ybanez on 12/11/22.
//

#include "../include/test_matrices.h"
#include "../include/asserts.h"
#include "../../math/include/matrix.h"

namespace tests::matrices {
    using namespace rt;

    void all() {
        set("Matrices", [] {
            init();
            compare();
        });
    }

    void init() {
        set("4 x 4 Matrix", [] {
            math::MatrixTable table{
                    {1,    2,    3,    4},
                    {5.5,  6.5,  7.5,  8.5},
                    {9,    10,   11,   12},
                    {13.5, 14.5, 15.5, 16.5}
            };
            math::Matrix matrix{table};

            ASSERT_EQ_MSG("[0, 0]", 1, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 3]", 4, matrix[0][3]);
            ASSERT_EQ_MSG("[1, 0]", 5.5, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 2]", 7.5, matrix[1][2]);
            ASSERT_EQ_MSG("[2, 2]", 11, matrix[2][2]);
            ASSERT_EQ_MSG("[3, 0]", 13.5, matrix[3][0]);
            ASSERT_EQ_MSG("[3, 2]", 15.5, matrix[3][2]);
        });
        set("2 x 2 Matrix", [] {
            math::MatrixTable table{
                    {-3, 5},
                    {1,  -2}
            };
            math::Matrix matrix{table};

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 1]", 5, matrix[0][1]);
            ASSERT_EQ_MSG("[1, 0]", 1, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
        });
        set("3 x 3 Matrix", [] {
            math::MatrixTable table{
                    {-3, 5,  0},
                    {1,  -2, -7},
                    {0,  1,  1}
            };
            math::Matrix matrix{table};

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
            ASSERT_EQ_MSG("[2, 2]", 1, matrix[2][2]);
        });
    }

    void compare() {
        set("Comparing matrices", [] {
            scenario("Identical matrices", [] {
                math::MatrixTable table1{
                        {1, 2, 3, 4},
                        {5, 6, 7, 8},
                        {9, 8, 7, 6},
                        {5, 4, 3, 2}
                };
                math::MatrixTable table2{
                        {1, 2, 3, 4},
                        {5, 6, 7, 8},
                        {9, 8, 7, 6},
                        {5, 4, 3, 2}
                };
                ASSERT_EQ(math::Matrix{table1}, math::Matrix{table2});
            });
            scenario("Non-identical matrices", [] {
                math::MatrixTable table1{
                        {1, 2, 3, 4},
                        {5, 6, 7, 8},
                        {9, 8, 7, 6},
                        {5, 4, 3, 2}
                };
                math::MatrixTable table2{
                        {2, 3, 4, 5},
                        {6, 7, 8, 9},
                        {8, 7, 6, 5},
                        {4, 3, 2, 1}
                };
                ASSERT_TRUE(math::Matrix{table1} != math::Matrix{table2});
            });
        });
    }
}