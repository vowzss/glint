#pragma once
namespace glint::engine::core::math::matrix {
    // --- methods ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr mat<T, C, R, I> mat<T, R, C, I>::transpose() const noexcept {
        mat<T, C, R, I> result{};
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[j][i] = data[i][j];
            }
        }
        return result;
    }

    /*
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I mat<T, R, C, I>::inverse() const noexcept {
        static_assert(rows == cols, "inverse only defined for square matrices!");
    }
    */
    /*
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr T mat<T, R, C, I>::determinant() const noexcept {
        static_assert(rows == cols, "determinant only defined for square matrices!");
    }
    */

    // --- getters / setters ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr vector::vec<T, C, void> mat<T, R, C, I>::row(size_t idx) const {
        vector::vec<T, C, void> result;
        for (size_t j = 0; j < cols; ++j) {
            result[j] = data[idx][j];
        }
        return result;
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr vector::vec<T, R, void> mat<T, R, C, I>::column(size_t idx) const {
        vector::vec<T, R, void> result;
        for (size_t i = 0; i < rows; ++i) {
            result[i] = data[i][idx];
        }
        return result;
    }

    // --- operators (mat-mat) ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I mat<T, R, C, I>::operator+(const mat& other) const noexcept {
        mat<T, R, C, I> result;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return static_cast<I>(result);
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I mat<T, R, C, I>::operator-(const mat& other) const noexcept {
        mat<T, R, C, I> result;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] - other[i][j];
            }
        }
        return static_cast<I>(result);
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    template <size_t K>
    constexpr I mat<T, R, C, I>::operator*(const mat<T, C, K, I>& other) const noexcept {
        mat<T, R, K, I> result{};
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < K; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return static_cast<I>(result);
    }

    // --- operators (mat-scalar) ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I mat<T, R, C, I>::operator*(T s) const noexcept {
        mat<T, R, C, I> result;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] * s;
            }
        }
        return static_cast<I>(result);
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I mat<T, R, C, I>::operator/(T s) const noexcept {
        mat<T, R, C, I> result;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] / s;
            }
        }
        return static_cast<I>(result);
    }

    // --- operators (mat-mat compound assignment) ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I& mat<T, R, C, I>::operator+=(const mat& other) noexcept {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] += other[i][j];
            }
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I& mat<T, R, C, I>::operator-=(const mat& other) noexcept {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] -= other[i][j];
            }
        }
        return static_cast<I&>(*this);
    }

    // --- operators (mat-scalar compound assignment) ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I& mat<T, R, C, I>::operator*=(T s) noexcept {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] *= s;
            }
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr I& mat<T, R, C, I>::operator/=(T s) noexcept {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] /= s;
            }
        }
        return static_cast<I&>(*this);
    }

    // --- operators (comparison) ---
    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr bool mat<T, R, C, I>::operator==(const mat& other) const noexcept {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] != other[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T, std::size_t R, std::size_t C, typename I>
    constexpr bool mat<T, R, C, I>::operator!=(const mat& other) const noexcept {
        return !(*this == other);
    }
}