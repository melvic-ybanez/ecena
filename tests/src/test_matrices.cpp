//
// Created by Melvic Ybanez on 12/11/22.
//

#include <array>
#include "../include/test_matrices.h"
#include "../include/asserts.h"
#include "../../engine/math/include/math_utils.h"
#include "../../engine/math/include/matrix.h"

namespace rt::tests::matrices {
    void all() {
        set("Matrices", [] {
            init();
            compare();
            multiplications();
            transpose();
            determinants();
            submatrices();
            minors();
            cofactors();
            inversion();
        });
    }

    void init() {
        set("4 x 4 Matrix", [] {
            Matrix<4, 4> matrix{
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
            Matrix<2, 2> matrix{
                    {{{-3, 5},
                      {1, -2}}}
            };

            ASSERT_EQ_MSG("[0, 0]", -3, matrix[0][0]);
            ASSERT_EQ_MSG("[0, 1]", 5, matrix[0][1]);
            ASSERT_EQ_MSG("[1, 0]", 1, matrix[1][0]);
            ASSERT_EQ_MSG("[1, 1]", -2, matrix[1][1]);
        });
        set("3 x 3 Matrix", [] {
            Matrix<3, 3> matrix{
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
                Matrix<4, 4> m1{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };
                Matrix<4, 4> m2{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };

                ASSERT_EQ(m1, m2);
            });
            scenario("Non-identical matrices", [] {
                Matrix<4, 4> m1{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };
                Matrix<4, 4> m2{
                        {{{2, 3, 4, 5},
                          {6, 7, 8, 9},
                          {8, 7, 6, 5},
                          {4, 3, 2, 1}}}
                };

                ASSERT_TRUE(m1 != m2);
            });
        });
    }

    void multiplications() {
        set("Multiplication", [] {
            scenario("Matrix-matrix", [] {
                Matrix<4, 4> m1{
                        {{{1, 2, 3, 4},
                          {5, 6, 7, 8},
                          {9, 8, 7, 6},
                          {5, 4, 3, 2}}}
                };
                Matrix<4, 4> m2{
                        {{{-2, 1, 2, 3},
                          {3, 2, 1, -1},
                          {4, 3, 6, 5},
                          {1, 2, 7, 8}}}
                };
                Matrix<4, 4> expected{
                        {{{20, 22, 50, 48},
                          {44, 54, 114, 108},
                          {40, 58, 110, 102},
                          {16, 26, 46, 42}}}
                };

                ASSERT_EQ(expected, m1 * m2);
            });
            scenario("Matrix-tuple", [] {
                Matrix<4, 4> matrix{
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
                Matrix<4, 4> matrix{
                        {{{0, 1, 2, 4},
                          {1, 2, 4, 8},
                          {2, 4, 8, 6},
                          {4, 8, 16, 32}}}
                };
                auto result = matrix * math::matrix::identity<4, 4>();
                ASSERT_EQ(matrix, result);
            });
            scenario("A product and its inverse", [] {
                Matrix<4, 4> matrix{
                        {{{3, -9, 7, 3},
                          {3, -8, 2, -9},
                          {-4, 4, 4, 1},
                          {-6, 5, -1, 1}}}
                };
                Matrix<4, 4> matrix2{
                        {{{8, 2, 2, 2},
                          {3, -1, 7, 0},
                          {7, 0, 5, 4},
                          {6, -2, 0, 5}}}
                };
                const auto &product{matrix * matrix2};

                ASSERT_EQ(matrix, product * matrix2.inverse());
            });
        });
    }

    void transpose() {
        set("Transpose", [] {
            scenario("A non-id matrix", [] {
                Matrix<4, 4> matrix{
                        {{{0, 9, 3, 0},
                          {9, 8, 0, 8},
                          {1, 8, 5, 3},
                          {0, 0, 5, 8}}}
                };
                Matrix<4, 4> expected{
                        {{{0, 9, 1, 0},
                          {9, 8, 8, 0},
                          {3, 0, 5, 5},
                          {0, 8, 3, 8}}}
                };
                ASSERT_EQ(expected, matrix.transpose());
            });
            scenario("An identity matrix", [] {
                auto idm = matrix::identity<4, 4>();
                ASSERT_EQ(idm, idm.transpose());
            });
        });
    }

    void determinants() {
        set("Determinants", [] {
            scenario("2x2", [] {
                Matrix<2, 2> matrix{
                        {{{1, 5},
                          {-3, 2}}}
                };
                ASSERT_EQ(17, matrix.determinant());
            });
            set("3x3", [] {
                Matrix<3, 3> matrix{
                        {{{1, 2, 6},
                          {-5, 8, -4},
                          {2, 6, 4}}}
                };
                ASSERT_EQ_MSG("Cofactor(0, 0)", 56, matrix.cofactor(0, 0));
                ASSERT_EQ_MSG("Cofactor(0, 1)", 12, matrix.cofactor(0, 1));
                ASSERT_EQ_MSG("Cofactor(0, 2)", -46, matrix.cofactor(0, 2));
                ASSERT_EQ_MSG("Determinant", -196, matrix.determinant());
            });
            set("4z4", [] {
                Matrix<4, 4> matrix{
                        {{{-2, -8, 3, 5},
                          {-3, 1, 7, 3},
                          {1, 2, -9, 6},
                          {-6, 7, 7, -9}}}
                };
                ASSERT_EQ_MSG("Cofactor(0, 0)", 690, matrix.cofactor(0, 0));
                ASSERT_EQ_MSG("Cofactor(0, 1)", 447, matrix.cofactor(0, 1));
                ASSERT_EQ_MSG("Cofactor(0, 2)", 210, matrix.cofactor(0, 2));
                ASSERT_EQ_MSG("Cofactor(0, 2)", 51, matrix.cofactor(0, 3));
                ASSERT_EQ_MSG("Determinant", -4071, matrix.determinant());
            });
        });
    }

    void submatrices() {
        set("Submatrices", [] {
            scenario("3x3", [] {
                Matrix<3, 3> matrix{
                        {{{1, 5, 0},
                          {-3, 2, 7},
                          {0, 6, -3}}}
                };
                Matrix<2, 2> expected{
                        {{{-3, 2},
                          {0, 6}}}
                };
                ASSERT_EQ(expected, matrix.submatrix(0, 2));
            });
            scenario("4x4", [] {
                Matrix<4, 4> matrix{
                        {{{-6, 1, 1, 6},
                          {-8, 5, 8, 6},
                          {-1, 0, 8, 2},
                          {-7, 1, -1, 1}}}
                };
                Matrix<3, 3> expected{
                        {{{-6, 1, 6},
                          {-8, 8, 6},
                          {-7, -1, 1}}}
                };
                ASSERT_EQ(expected, matrix.submatrix(2, 1));
            });
        });
    }

    void minors() {
        set("Computing a minor of a 3x3 matrix", [] {
            Matrix<3, 3> matrix{
                    {{{3, 5, 0},
                      {2, -1, -7},
                      {6, -1, 5}}}
            };
            ASSERT_EQ_MSG("Check determinant", 25, matrix.submatrix(1, 0).determinant());
            ASSERT_EQ_MSG("Minor(1, 0)", 25, matrix.minor(1, 0));
        });
    }

    void cofactors() {
        set("Calculating the cofactor of a 3x3 matrix", [] {
            Matrix<3, 3> matrix{
                    {{{3, 5, 0},
                      {2, -1, -7},
                      {6, -1, 5}}}
            };
            ASSERT_EQ_MSG("Minor(0, 0)", -12, matrix.minor(0, 0));
            ASSERT_EQ_MSG("Cofactor(0, 0)", -12, matrix.cofactor(0, 0));
            ASSERT_EQ_MSG("Minor(1, 0)", 25, matrix.minor(1, 0));
            ASSERT_EQ_MSG("Cofactor(1, 0)", -25, matrix.cofactor(1, 0));
        });
    }

    void inversion() {
        set("Inversion", [] {
            set("Testing an invertible matrix for invertibility", [] {
                Matrix<4, 4> matrix{
                        {{{6, 4, 4, 4},
                          {5, 5, 7, 6},
                          {4, -9, 3, -7},
                          {9, 1, 7, -6}}}
                };
                ASSERT_EQ_MSG("Check determinant", -2120, matrix.determinant());
                ASSERT_TRUE_MSG("Is invertible", matrix.is_invertible());
            });
            set("Testing a non-invertible matrix for invertibility", [] {
                Matrix<4, 4> matrix{
                        {{{-4, 2, -2, -3},
                          {9, 6, 2, 6},
                          {0, -5, 1, -5},
                          {0, 0, 0, 0}}}
                };
                ASSERT_EQ_MSG("Check determinant", 0, matrix.determinant());
                ASSERT_TRUE_MSG("Is not invertible", !matrix.is_invertible());
            });
            set("Calculating the inverse of a matrix", [] {
                Matrix<4, 4> matrix{
                        {{{-5, 2, 6, -8},
                          {1, -5, 1, 8},
                          {7, 7, -6, -7},
                          {1, -3, 7, 4}}}
                };
                Matrix<4, 4> m_inverse{matrix.inverse()};

                ASSERT_EQ_MSG("Check determinant", 532, matrix.determinant());
                ASSERT_EQ_MSG("Cofactor(2, 3)", -160, matrix.cofactor(2, 3));
                ASSERT_EQ_MSG("Inverse(3, 2)", -160.0 / 532, m_inverse[3][2]);
                ASSERT_EQ_MSG("Cofactor(3, 2)", 105, matrix.cofactor(3, 2));
                ASSERT_EQ_MSG("Inverse(2, 3)", 105.0 / 532, m_inverse[2][3]);

                Matrix<4, 4> m_expected_inverse{
                        {{{0.21805, 0.45113, 0.24060, -0.04511},
                          {-0.80827, -1.45677, -0.44361, 0.52068},
                          {-0.07895, -0.22368, -0.05263, 0.19737},
                          {-0.52256, -0.81391, -0.30075, 0.30639}}}
                };

                Matrix<4, 4> matrix1{
                        {{{8, -5, 9, 2},
                          {7, 5, 6, 1},
                          {-6, 0, 9, 6},
                          {-3, 0, -9, -4}}}
                };
                Matrix<4, 4> m1_expected_inverse{
                        {{{-0.15385, -0.15385, -0.28205, -0.53846},
                          {-0.07692, 0.12308, 0.02564, 0.03077},
                          {0.35897, 0.35897, 0.43590, 0.92308},
                          {-0.69231, -0.69231, -0.76923, -1.92308}}}
                };

                Matrix<4, 4> matrix2{
                        {{{9, 3, 0, 9},
                          {-5, -2, -6, -3},
                          {-4, 9, 6, 4},
                          {-7, 6, 6, 2}}}
                };
                Matrix<4, 4> m2_expected_inverse{
                        {{{-0.04074, -0.07778, 0.14444, -0.22222},
                          {-0.07778, 0.03333, 0.36667, -0.33333},
                          {-0.02901, -0.14630, -0.10926, 0.12963},
                          {0.17778, 0.06667, -0.26667, 0.33333}}}
                };

                ASSERT_TRUE_MSG("Compute inverses", m_inverse == m_expected_inverse
                                                    && matrix1.inverse() == m1_expected_inverse &&
                                                    matrix2.inverse() == m2_expected_inverse);
            });
        });
    }
}