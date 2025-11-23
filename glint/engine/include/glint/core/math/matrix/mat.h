#pragma once

#include <array>
#include <cstddef>
#include <ostream>

#include "../vectors/vec.h"

template <typename T, std::size_t R, std::size_t C, typename I>
struct mat {
    static_assert(std::is_arithmetic_v<T>, "must be an arithmetic type!");

    static constexpr size_t rows = R;
    static constexpr size_t cols = C;

  protected:
    std::array<std::array<T, C>, R> data;

  protected:
    // --- constructors ---
    constexpr mat() : data{} {};
    constexpr mat(const std::array<T, R * C>& values) {
        // static_assert(values.size() == R * C, "array size must match matrix size!");
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = values[i * cols + j];
            }
        }
    }

  public:
    // --- methods ---
    constexpr mat<T, C, R, I> transpose() const noexcept;
    // constexpr I inverse() const noexcept;
    // constexpr T determinant() const noexcept;

    // --- getters / setters ---
    constexpr vec<T, C, void> row(size_t r) const;
    constexpr vec<T, R, void> column(size_t c) const;

    // --- operators ---
    T* raw() { return &data[0][0]; }
    const T* raw() const { return &data[0][0]; }

    std::array<T, C>& operator[](size_t row) { return data[row]; }
    const std::array<T, C>& operator[](size_t row) const { return data[row]; }

    // --- operators (mat-mat) ---
    constexpr I operator+(const mat& other) const noexcept;
    constexpr I operator-(const mat& other) const noexcept;
    template <size_t K>
    constexpr I operator*(const mat<T, C, K, I>& other) const noexcept;

    // --- operators (mat-scalar) ---
    constexpr I operator*(T s) const noexcept;
    constexpr I operator/(T s) const noexcept;

    // --- operators (mat-mat compound assignment) ---
    constexpr I& operator+=(const mat& other) noexcept;
    constexpr I& operator-=(const mat& other) noexcept;

    // --- operators (mat-scalar compound assignment) ---
    constexpr I& operator*=(T s) noexcept;
    constexpr I& operator/=(T s) noexcept;

    // --- operators (comparison) ---
    constexpr bool operator==(const mat& other) const noexcept;
    constexpr bool operator!=(const mat& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const mat& m) {
        for (size_t i = 0; i < rows; ++i) {
            os << m.row(i) << "\n";
        }
        return os;
    }
};

#include "mat.inl"