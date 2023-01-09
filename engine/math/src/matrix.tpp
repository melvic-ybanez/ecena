#include <optional>

namespace rt::math::matrix {
    template<size_t R, size_t C>
    Row<C> &Matrix<R, C>::operator[](size_t row) {
        return elems_[row];
    }

    template<size_t R, size_t C>
    const Row<C> &Matrix<R, C>::operator[](size_t row) const {
        return elems_[row];
    }

    template<size_t R, size_t C>
    bool Matrix<R, C>::operator==(const Matrix<R, C> &other) const {
        bool equals = true;
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C; c++) {
                if (!compare_reals(this->elems_[r][c], other.elems_[r][c])) {
                    equals = false;
                    break;
                }
            }
            if (!equals) break;
        }
        return equals;
    }

    template<size_t R, size_t C>
    template<size_t R1, size_t C1>
    bool Matrix<R, C>::operator!=(const Matrix<R1, C1> &that) const {
        return !(*this == that);
    }

    template<size_t R, size_t C>
    const Table<R, C> &Matrix<R, C>::elems() const {
        return elems_;
    }

    template<size_t R, size_t C>
    template<size_t C0>
    Matrix<R, C0> Matrix<R, C>::operator*(const Matrix<C, C0> &that) const {
        Matrix<R, C0> product;
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C0; c++) {
                real sum = 0;
                for (auto j = 0; j < C; j++) {
                    sum += elems()[r][j] * that.elems()[j][c];
                }
                product[r][c] = sum;
            }
        }
        return product;
    }

    template<size_t R, size_t C>
    Matrix<R - 1, C - 1> Matrix<R, C>::submatrix(size_t row, size_t col) const {
        Matrix<R - 1, C - 1> result;
        auto r0 = 0;
        for (auto r = 0; r < R; r++) {
            if (r == row) continue;
            auto c0 = 0;
            for (auto c = 0; c < C; c++) {
                if (c == col) continue;
                result[r0][c0] = (*this)[r][c];
                c0++;
            }
            r0++;
        }
        return result;
    }

    template<size_t R, size_t C>
    real Matrix<R, C>::determinant() const {
        real determinant = 0;
        for (auto c = 0; c < C; c++) {
            determinant += (*this)[0][c] * cofactor(0, c);
        }
        return determinant;
    }

    template<size_t R, size_t C>
    real Matrix<R, C>::minor(size_t row, size_t col) const {
        return submatrix(row, col).determinant();
    }

    template<size_t R, size_t C>
    real Matrix<R, C>::cofactor(size_t row, size_t col) const {
        auto m = minor(row, col);
        if ((row + col) % 2 != 0) return -m;
        return m;
    }

    template<size_t R, size_t C>
    bool Matrix<R, C>::is_invertible() const {
        return determinant() != 0;
    }

    template<size_t R, size_t C>
    Matrix<C, R> Matrix<R, C>::transpose() const {
        Matrix<C, R> result;
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C; c++) {
                result[c][r] = (*this)[r][c];
            }
        }
        return result;
    }

    template<size_t R, size_t C>
    Matrix<C, R> Matrix<R, C>::inverse() const {
        if (!is_invertible()) return *this;

        Matrix<R, C> cofactors;
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C; c++) {
                cofactors[r][c] = cofactor(r, c);
            }
        }

        Matrix<C, R> transpose{cofactors.transpose()};
        auto d{determinant()};

        Matrix<C, R> result;

        // Divide each transposed cofactor by the original's determinant.
        // Note that we need to reverse the column and row in the loop condition
        // to account for the result of the transposition.
        for (auto r = 0; r < C; r++) {
            for (auto c = 0; c < R; c++) {
                result[r][c] = transpose[r][c] / d;
            }
        }

        return result;
    }

    template<size_t R, size_t C>
    std::ostream &operator<<(std::ostream &out, const Matrix<R, C> &matrix) {
        for (const auto &row: matrix.elems()) {
            for (const auto &elem: row) {
                out << elem << ' ';
            }
            out << std::endl;
        }
        return out;
    }

    template<size_t R>
    Tuple operator*(const Matrix<R, 4> &matrix, const Tuple &tuple) {
        Matrix<4, 1> other{{{{tuple.x()}, {tuple.y()}, {tuple.z()}, {tuple.w()}}}};
        Matrix<R, 1> result = matrix * other;
        return Tuple{result[0][0], result[0][1], result[0][2], result[0][3]};
    }

    template<size_t R, size_t C>
    Matrix<R, C> identity() {
        static std::optional<Matrix<R, C>> opt_identity;

        if (opt_identity.has_value()) return opt_identity.value();

        opt_identity = {{}};
        auto &identity_ = opt_identity.value();

        // set all the elements along the diagonal to 1
        for (auto r = 0; r < R; r++) {
            for (auto c = 0; c < C; c++) {
                if (r == c) identity_[r][c] = 1;
            }
        }

        return identity_;
    }
}