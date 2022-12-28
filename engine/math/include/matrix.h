//
// Created by Melvic Ybanez on 12/11/22.
//

#ifndef ECENA_MATRIX_H
#define ECENA_MATRIX_H

#include <array>
#include <ostream>
#include "math_utils.h"
#include "tuples.h"

namespace rt::math::matrix {
    template<size_t S>
    using Row = std::array<real, S>;

    template<size_t R, size_t C>
    using Table = std::array<Row<C>, R>;

    template<size_t R, size_t C>
    class Matrix {
        Table<R, C> elems_;

    public:
        explicit Matrix(Table<R, C> elems) : elems_{std::move(elems)} {}

        Matrix() = default;

        Row<C> &operator[](size_t row);

        const Row<C> &operator[](size_t row) const;

        bool operator==(const Matrix<R, C> &other) const;

        template<size_t R1, size_t C1>
        bool operator!=(const Matrix<R1, C1> &other) const;

        const Table<R, C> &elems() const;

        /**
         * Multiply this matrix by another matrix, provided that the former's number of columns
         * is the same as the latter's number of rows.
         */
        template<size_t C0>
        Matrix<R, C0> operator*(const Matrix<C, C0> &other) const;

        Matrix<R - 1, C - 1> submatrix(size_t row, size_t col) const;

        /**
         * Computes the determinant of the matrix assuming the dimensions
         * are above 2x2.
         * Note: 2x2 matrices are template-specialized.
         */
        real determinant() const;

        /**
         * Computes the determinants of the submatrix
         */
        real minor(size_t row, size_t col) const;

        /**
         * A cofactor is a minor that has its sign possibly flipped according to the
         * following table
         * [+ - +]
         * [- + -]
         * [+ - +]
         * If the minor lands on a positive spot, it gets to keep its value. Otherwise, its
         * sign is negated.
         */
        real cofactor(size_t row, size_t col) const;

        bool is_invertible() const;

        Matrix<C, R> transpose() const;

        Matrix<C, R> inverse() const;
    };  // end of Matrix

    template<size_t R, size_t C>
    std::ostream &operator<<(std::ostream &out, const Matrix<R, C> &matrix);

    template<size_t R>
    Tuple operator*(const Matrix<R, 4> &matrix, const Tuple &tuple);

    template<size_t R, size_t C>
    Matrix<R, C> identity();

    /**
     * Computes the determinants of any 2x2 matrix.
     */
    template<>
    real Matrix<2, 2>::determinant() const;

    Matrix<4, 4> translation(real x, real y, real z);

    Matrix<4, 4> scaling(real x, real y, real z);

    Matrix<4, 4> rotation_x(real r);

    Matrix<4, 4> rotation_y(real r);

    Matrix<4, 4> rotation_z(real r);

    /**
     * Moves x in proportion to y and z.
     * Moves y in proportion to x and z.
     * Moves z in proportion to x and y.
     */
    Matrix<4, 4> shearing(real xy, real xz, real yx, real yz, real zx, real zy);
}

namespace rt::math {
    template<size_t R, size_t C>
    using Matrix = rt::math::matrix::Matrix<R, C>;
}

#include "matrix.tpp"

#endif //ECENA_MATRIX_H