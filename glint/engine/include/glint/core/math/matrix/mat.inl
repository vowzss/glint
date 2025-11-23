#pragma once

// --- constructors ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>::mat(const std::array<T, R * C>& values) {
    // static_assert(values.size() == R * C, "array size must match matrix size!");

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = values[i * cols + j];
        }
    }
}

// --- methods ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, C, R> mat<T, R, C>::transpose() const noexcept {
    mat<T, C, R> result{};
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = data[i][j];
        }
    }
    return result;
}

template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::inverse() const noexcept {
    static_assert(rows == cols, "inverse only defined for square matrices!");

    if constexpr (rows == 2) {
        T det = determinant();
        assert(det != 0);
        // clang-format off
        return mat<T, 2, 2>({
            data[1][1] / det, -data[0][1] / det, 
            -data[1][0] / det, data[0][0] / det
        });
        // clang-format on
    } else if constexpr (rows == 3) {
        T det = determinant();
        assert(det != 0);
        // clang-format off
         return mat<T,3,3>({
             (data[1][1] * data[2][2] - data[1][2] * data[2][1]) / det,
            -(data[0][1] * data[2][2] - data[0][2] * data[2][1]) / det,
             (data[0][1] * data[1][2] - data[0][2] * data[1][1]) / det,

            -(data[1][0] * data[2][2] - data[1][2] * data[2][0]) / det,
             (data[0][0] * data[2][2] - data[0][2] * data[2][0]) / det,
            -(data[0][0] * data[1][2] - data[0][2] * data[1][0]) / det,

             (data[1][0] * data[2][1] - data[1][1] * data[2][0]) / det,
            -(data[0][0] * data[2][1] - data[0][1] * data[2][0]) / det,
             (data[0][0] * data[1][1] - data[0][1] * data[1][0]) / det
        });
        // clang-format on
    } else {
        static_assert(rows <= 3, "inverse for R>3 not implemented yet!");
    }
}

template <typename T, std::size_t R, std::size_t C>
constexpr T mat<T, R, C>::determinant() const noexcept {
    static_assert(rows == cols, "determinant only defined for square matrices!");

    if constexpr (rows == 2) {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    } else if constexpr (rows == 3) {
        // clang-format off
        return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])
             - data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0])
             + data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
        // clang-format on
    } else {
        static_assert(rows <= 3, "determinant for R>3 not implemented yet!");
    }
}

// --- getters / setters ---
template <typename T, std::size_t R, std::size_t C>
constexpr vec<T, C, void> mat<T, R, C>::row(size_t idx) const {
    vec<T, C, void> result;
    for (size_t j = 0; j < cols; ++j) {
        result[j] = data[idx][j];
    }
    return result;
}

template <typename T, std::size_t R, std::size_t C>
constexpr vec<T, R, void> mat<T, R, C>::column(size_t idx) const {
    vec<T, R, void> result;
    for (size_t i = 0; i < rows; ++i) {
        result[i] = data[i][idx];
    }
    return result;
}

// --- operators (mat-mat) ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator+(const mat& other) const noexcept {
    mat<T, R, C> result;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] + other[i][j];
        }
    }
    return result;
}

template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator-(const mat& other) const noexcept {
    mat<T, R, C> result;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] - other[i][j];
        }
    }
    return result;
}

template <typename T, std::size_t R, std::size_t C>
template <size_t K>
constexpr mat<T, R, K> mat<T, R, C>::operator*(const mat<T, C, K>& other) const noexcept {
    mat<T, R, K> result(T(0));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < K; ++j) {
            for (size_t k = 0; k < cols; ++k) {
                result[i][j] += data[i][k] * other[k][j];
            }
        }
    }
    return result;
}

// --- operators (mat-vector) ---
template <typename T, std::size_t R, std::size_t C>
constexpr vec<T, R, void> mat<T, R, C>::operator*(const vec<T, C, void>& v) const noexcept {
    vec<T, R, void> result;
    for (size_t i = 0; i < rows; ++i) {
        result[i] = T(0);
        for (size_t j = 0; j < cols; ++j) {
            result[i] += data[i][j] * v[j];
        }
    }
    return result;
}

// --- operators (mat-scalar) ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator*(T s) const noexcept {
    mat<T, R, C> result;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] * s;
        }
    }
    return result;
}

template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C> mat<T, R, C>::operator/(T s) const noexcept {
    mat<T, R, C> result;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = data[i][j] / s;
        }
    }
    return result;
}

// --- operators (mat-mat compound assignment) ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator+=(const mat& other) noexcept {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] += other[i][j];
        }
    }
    return *this;
}

template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator-=(const mat& other) noexcept {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] -= other[i][j];
        }
    }
    return *this;
}

// --- operators (mat-scalar compound assignment) ---
template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator*=(T s) noexcept {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] *= s;
        }
    }
    return *this;
}

template <typename T, std::size_t R, std::size_t C>
constexpr mat<T, R, C>& mat<T, R, C>::operator/=(T s) noexcept {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] /= s;
        }
    }
    return *this;
}

// --- operators (comparison) ---
template <typename T, std::size_t R, std::size_t C>
constexpr bool mat<T, R, C>::operator==(const mat& other) const noexcept {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (data[i][j] != other[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <typename T, std::size_t R, std::size_t C>
constexpr bool mat<T, R, C>::operator!=(const mat& other) const noexcept {
    return !(*this == other);
}
