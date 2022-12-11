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
        });
    }

    void init() {
        set("Constructing and inspecting a 4x4 matrix", [] {
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
    }
}